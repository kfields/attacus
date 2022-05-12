// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>


#include "json.h"
#include "method_call.h"
#include "method_codec.h"

namespace attacus {

// An implementation of MethodCodec that uses JSON strings as the serialization.
class JsonMethodCodec : public MethodCodec<JsonValue> {
 public:
  // Returns the shared instance of the codec.
  static const JsonMethodCodec& GetInstance();

  ~JsonMethodCodec() = default;

  // Prevent copying.
  JsonMethodCodec(JsonMethodCodec const&) = delete;
  JsonMethodCodec& operator=(JsonMethodCodec const&) = delete;

 protected:
  // Instances should be obtained via GetInstance.
  JsonMethodCodec() = default;

  // |flutter::MethodCodec|
  std::unique_ptr<MethodCall<JsonValue>> DecodeMethodCallInternal(
      const uint8_t* message,
      const size_t message_size) const override;

  // |flutter::MethodCodec|
  std::unique_ptr<std::vector<uint8_t>> EncodeMethodCallInternal(
      const MethodCall<JsonValue>& method_call) const override;

  // |flutter::MethodCodec|
  std::unique_ptr<std::vector<uint8_t>> EncodeSuccessEnvelopeInternal(
      const JsonValue* result) const override;

  // |flutter::MethodCodec|
  std::unique_ptr<std::vector<uint8_t>> EncodeErrorEnvelopeInternal(
      const std::string& error_code,
      const std::string& error_message,
      const JsonValue* error_details) const override;

  // |flutter::MethodCodec|
  bool DecodeAndProcessResponseEnvelopeInternal(
      const uint8_t* response,
      const size_t response_size,
      MethodResult<JsonValue>* result) const override;
};

}  // namespace attacus
