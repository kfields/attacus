#pragma once

#include "flutter_messenger.h"

namespace attacus {

class Channel {
public:
    // Creates an instance that sends and receives method calls on the channel
    // named |name|, encoded with |codec| and dispatched via |messenger|.
    Channel(FlutterMessenger& messenger, const std::string& name) : messenger_(&messenger), name_(name) {
        messenger.AddChannel(*this);
    }
    virtual void HandleMessage(const FlutterPlatformMessage& message, BinaryReply reply_handler) = 0;
    //Data members
    FlutterMessenger* messenger_;
    std::string name_;
};

} //namespace attacus