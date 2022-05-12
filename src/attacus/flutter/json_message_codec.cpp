// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <iostream>
#include <string>
#include "json_message_codec.h"
#include "json.h"

namespace attacus
{
// static
const JsonMessageCodec &JsonMessageCodec::GetInstance()
{
    static JsonMessageCodec sInstance;
    return sInstance;
}

std::unique_ptr<std::vector<uint8_t>> JsonMessageCodec::EncodeMessageInternal(
    const JsonValue &message) const
{

    std::string buffer = message.dump();
    const char *buffer_start = buffer.c_str();
    return std::make_unique<std::vector<uint8_t>>(
        buffer_start, buffer_start + buffer.size());
}

std::unique_ptr<JsonValue> JsonMessageCodec::DecodeMessageInternal(
    const uint8_t *binary_message,
    const size_t message_size) const
{

    auto raw_message = reinterpret_cast<const char *>(binary_message);
    auto json_message = JsonValue::parse(raw_message, raw_message + message_size);
    std::cout << json_message;

    return std::make_unique<JsonValue>(json_message);
}

} // namespace attacus
