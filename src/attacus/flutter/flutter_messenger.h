// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <functional>
#include <string>
#include <map>

#include "flutter_embedder.h"

namespace attacus {

class FlutterView;
class Channel;

// A binary message reply callback.
//
// Used for submitting a binary reply back to a Flutter message sender.
typedef std::function<void(const uint8_t* reply, size_t reply_size)>
    BinaryReply;

// A message handler callback.
//
// Used for receiving messages from Flutter and providing an asynchronous reply.
typedef std::function<
    void(const uint8_t* message, size_t message_size, BinaryReply reply)>
    BinaryMessageHandler;

class FlutterMessenger {
public:
    explicit FlutterMessenger(FlutterView& view);

    virtual ~FlutterMessenger();

    // Prevent copying.
    FlutterMessenger(FlutterMessenger const&) = delete;
    FlutterMessenger& operator=(FlutterMessenger const&) = delete;

    void Create();

    // |flutter::FlutterMessenger|
    void Send(const std::string& channel,
        const uint8_t* message,
        size_t message_size,
        BinaryReply reply) const;
    
    void Receive(const FlutterPlatformMessage& message);

    void AddChannel(Channel& channel);

private:
    // Handle for interacting with the C API.
    FlutterView* view_;
    FlutterEngine engine_ = nullptr;
    std::map<std::string, Channel*> channels_;
};

}  // namespace attacus
