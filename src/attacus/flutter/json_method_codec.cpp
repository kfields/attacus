// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <iostream>
#include <string>

#include "json_method_codec.h"
#include "json_message_codec.h"

namespace attacus
{

// Keys used in MethodCall encoding.
constexpr char kMessageMethodKey[] = "method";
constexpr char kMessageArgumentsKey[] = "args";

// static
const JsonMethodCodec &JsonMethodCodec::GetInstance()
{
    static JsonMethodCodec sInstance;
    return sInstance;
}

std::unique_ptr<MethodCall<JsonValue>>
JsonMethodCodec::DecodeMethodCallInternal(const uint8_t *message, size_t message_size) const {
    std::unique_ptr<JsonValue> json_message_ptr = JsonMessageCodec::GetInstance().DecodeMessage(message, message_size);
    JsonValue& json_message = *json_message_ptr;
    if (!json_message_ptr)
    {
        return nullptr;
    }
    std::string method_name = json_message[kMessageMethodKey];
    auto arguments = new JsonValue(json_message[kMessageArgumentsKey]);

    return std::make_unique<MethodCall<JsonValue>>(method_name, std::unique_ptr<JsonValue>(arguments));
}

std::unique_ptr<std::vector<uint8_t>> JsonMethodCodec::EncodeMethodCallInternal(
    const MethodCall<JsonValue> &method_call) const
{
    JsonValue message = JsonValue::object(
        {
            { kMessageMethodKey, method_call.method_name() },
            { kMessageArgumentsKey, *method_call.arguments() }
        }
    );
    return JsonMessageCodec::GetInstance().EncodeMessage(message);
}

std::unique_ptr<std::vector<uint8_t>>
JsonMethodCodec::EncodeSuccessEnvelopeInternal(
    const JsonValue *result) const
{
    JsonValue envelope = JsonValue::array({*result});
    /*
    envelope.SetArray();
    rapidjson::Value result_value;
    if (result)
    {
        result_value.CopyFrom(*result, envelope.GetAllocator());
    }
    envelope.PushBack(result_value, envelope.GetAllocator());

    return JsonMessageCodec::GetInstance().EncodeMessage(envelope);
    */
   return JsonMessageCodec::GetInstance().EncodeMessage(envelope);
   //return nullptr;
}

std::unique_ptr<std::vector<uint8_t>>
JsonMethodCodec::EncodeErrorEnvelopeInternal(
    const std::string &error_code,
    const std::string &error_message,
    const JsonValue *error_details) const
{
    // NOLINTNEXTLINE(clang-analyzer-core.NullDereference)
    /*
    JsonValue envelope(rapidjson::kArrayType);
    auto &allocator = envelope.GetAllocator();
    envelope.PushBack(rapidjson::Value(error_code, allocator), allocator);
    envelope.PushBack(rapidjson::Value(error_message, allocator), allocator);
    rapidjson::Value details_value;
    if (error_details)
    {
        details_value.CopyFrom(*error_details, allocator);
    }
    envelope.PushBack(details_value, allocator);

    return JsonMessageCodec::GetInstance().EncodeMessage(envelope);
    */
   return nullptr;
}

bool JsonMethodCodec::DecodeAndProcessResponseEnvelopeInternal(
    const uint8_t *response,
    size_t response_size,
    MethodResult<JsonValue> *result) const
{
    std::unique_ptr<JsonValue> json_response =
        JsonMessageCodec::GetInstance().DecodeMessage(response, response_size);
    if (!json_response)
    {
        return false;
    }
    if (!json_response->is_array())
    {
        return false;
    }
    switch (json_response->size())
    {
    case 1:
    {
        auto value = json_response->at(0);
        if (value.is_null())
        {
            result->Success();
        }
        else
        {
            result->Success(value);
        }
        return true;
    }
    case 3:
    {
        std::string code = json_response->at(0).get<std::string>();
        std::string message = json_response->at(1).get<std::string>();
        auto details = json_response->at(2);
        if (details.is_null())
        {
            result->Error(code, message);
        }
        else
        {
            result->Error(code, message, details);
        }
        return true;
    }
    default:
        return false;
    }

    /*
    switch (json_response->Size())
    {
    case 1:
    {
        std::unique_ptr<JsonValue> value =
            ExtractElement(json_response.get(), &((*json_response)[0]));
        if (value->IsNull())
        {
            result->Success();
        }
        else
        {
            result->Success(*value);
        }
        return true;
    }
    case 3:
    {
        std::string code = (*json_response)[0].GetString();
        std::string message = (*json_response)[1].GetString();
        std::unique_ptr<JsonValue> details =
            ExtractElement(json_response.get(), &((*json_response)[2]));
        if (details->IsNull())
        {
            result->Error(code, message);
        }
        else
        {
            result->Error(code, message, *details);
        }
        return true;
    }
    default:
        return false;
    }
    */
   //TODO:
   //return false;
}

} // namespace attacus
