// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>

#include "message_codec.h"
#include "json.h"

namespace attacus {

// A message encoding/decoding mechanism for communications to/from the
// Flutter engine via JSON channels.
class JsonMessageCodec : public MessageCodec<JsonValue> {
 public:
  // Returns the shared instance of the codec.
  static const JsonMessageCodec& GetInstance();

  ~JsonMessageCodec() = default;

  // Prevent copying.
  JsonMessageCodec(JsonMessageCodec const&) = delete;
  JsonMessageCodec& operator=(JsonMessageCodec const&) = delete;

 protected:
  // Instances should be obtained via GetInstance.
  JsonMessageCodec() = default;

  // |flutter::MessageCodec|
  std::unique_ptr<JsonValue> DecodeMessageInternal(
      const uint8_t* binary_message,
      const size_t message_size) const override;

  // |flutter::MessageCodec|
  std::unique_ptr<std::vector<uint8_t>> EncodeMessageInternal(
      const JsonValue& message) const override;
};

}  // namespace attacus
