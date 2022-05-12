// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_SHELL_PLATFORM_COMMON_CLIENT_WRAPPER_INCLUDE_FLUTTER_METHOD_CHANNEL_H_
#define FLUTTER_SHELL_PLATFORM_COMMON_CLIENT_WRAPPER_INCLUDE_FLUTTER_METHOD_CHANNEL_H_

#include <iostream>
#include <string>

#include "channel.h"
#include "method.h"

namespace attacus
{

// A channel for communicating with the Flutter engine using invocation of
// asynchronous methods.
template <typename T = EncodableValue>
class MethodChannel : public Channel
{
public:
    // Creates an instance that sends and receives method calls on the channel
    // named |name|, encoded with |codec| and dispatched via |messenger|.
    MethodChannel(FlutterMessenger &messenger,
                    const std::string &name,
                    const MethodCodec<T> *codec)
        : Channel(messenger, name), codec_(codec) {}

    ~MethodChannel() = default;

    // Prevent copying.
    MethodChannel(MethodChannel const &) = delete;
    MethodChannel &operator=(MethodChannel const &) = delete;
    //
    void AddMethod(Method<T> &method) {
        methods_[method.name_] = &method;
    }
    MethodChannel<T>& def(const std::string &name, const MethodCallHandler<T> call_handler) {
        new Method<T>(*this, name, call_handler);
        return *this;
    }
    //
    virtual void HandleMessage(const FlutterPlatformMessage& message, BinaryReply reply_handler) {
        // Use this channel's codec to decode the call and build a result handler.
        auto result = std::make_unique<EngineMethodResult<T>>(std::move(reply_handler), codec_);
        std::unique_ptr<MethodCall<T>> method_call = codec_->DecodeMethodCall(message.message, message.message_size);
        if (!method_call)
        {
            std::cerr << "Unable to construct method call from message on channel "
                        << name_ << std::endl;
            result->NotImplemented();
            return;
        }
        //call_handler_(*method_call, std::move(result));
        const std::string method_name = method_call->method_name();
        if (methods_.find(method_name) == methods_.end())
        {
            std::cout << "Unknown Method: " << method_name << std::endl;
            result->NotImplemented();
            return;
        }
        methods_[method_name]->HandleCall(*method_call, std::move(result));

    }

    // Sends a message to the Flutter engine on this channel.
    //
    // If |result| is provided, one of its methods will be invoked with the
    // response from the engine.
    void InvokeMethod(const std::string &method,
                        std::unique_ptr<T> arguments,
                        std::unique_ptr<MethodResult<T>> result = nullptr)
    {
        MethodCall<T> method_call(method, std::move(arguments));
        std::unique_ptr<std::vector<uint8_t>> message =
            codec_->EncodeMethodCall(method_call);
        if (!result)
        {
            messenger_->Send(name_, message->data(), message->size(), nullptr);
            return;
        }

        // std::function requires a copyable lambda, so convert to a shared pointer.
        // This is safe since only one copy of the shared_pointer will ever be
        // accessed.
        std::shared_ptr<MethodResult<T>> shared_result(result.release());
        const auto *codec = codec_;
        std::string channel_name = name_;
        BinaryReply reply_handler = [shared_result, codec, channel_name](
                                        const uint8_t *reply, size_t reply_size)
        {
            if (reply_size == 0)
            {
                shared_result->NotImplemented();
                return;
            }
            // Use this channel's codec to decode and handle the
            // reply.
            bool decoded = codec->DecodeAndProcessResponseEnvelope(
                reply, reply_size, shared_result.get());
            if (!decoded)
            {
                std::cerr << "Unable to decode reply to method "
                                "invocation on channel "
                            << channel_name << std::endl;
                shared_result->NotImplemented();
            }
        };

        messenger_->Send(name_, message->data(), message->size(),
                            std::move(reply_handler));
    }

private:
    const MethodCodec<T> *codec_;
    std::map<std::string, Method<T>*> methods_;
};

//TODO:?
template <typename T>
Method<T>::Method(MethodChannel<T> &channel,
            const std::string &name,
            const MethodCallHandler<T> call_handler)
: channel_(&channel), name_(name), call_handler_(call_handler) {
    channel.AddMethod(*this);
}

} // namespace attacus

#endif // FLUTTER_SHELL_PLATFORM_COMMON_CLIENT_WRAPPER_INCLUDE_FLUTTER_METHOD_CHANNEL_H_
