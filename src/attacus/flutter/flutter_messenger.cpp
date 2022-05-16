#include <cassert>
#include <iostream>
#include <variant>

#include "flutter_embedder.h"
#include "flutter_messenger.h"
#include "flutter_window.h"
#include "engine_method_result.h"
#include "channel.h"

namespace attacus
{

FlutterMessenger::FlutterMessenger(FlutterWindow& view)
    : view_(&view) {}

FlutterMessenger::~FlutterMessenger() = default;

void FlutterMessenger::Create()
{
    engine_ = view_->engine_;
}

void FlutterMessenger::Send(const std::string &channel,
                            const uint8_t *message,
                            size_t message_size,
                            BinaryReply reply) const
{
    FlutterPlatformMessage platformMessage = {0};

    platformMessage.struct_size = sizeof(FlutterPlatformMessage);
    platformMessage.channel = channel.c_str();
    platformMessage.message = message;
    platformMessage.message_size = message_size;

    if (reply == nullptr)
    {
        FlutterEngineResult result = FlutterEngineSendPlatformMessage(engine_, &platformMessage);
        return;
    }
    struct Captures
    {
        BinaryReply reply;
    };
    auto captures = new Captures();
    captures->reply = reply;

    auto message_reply = [](const uint8_t *data, size_t data_size,
                            void *user_data)
    {
        auto captures = reinterpret_cast<Captures *>(user_data);
        captures->reply(data, data_size);
        delete captures;
    };
    // bool result = FlutterDesktopMessengerSendWithReply(messenger_, channel.c_str(), message, message_size, message_reply, captures);
    bool result = true;
    if (!result)
    {
        delete captures;
    }
}

/*
FlutterEngineResult FlutterPlatformMessageCreateResponseHandle(
    FLUTTER_API_SYMBOL(FlutterEngine) engine,
    FlutterDataCallback data_callback,
    void* user_data,
    FlutterPlatformMessageResponseHandle** response_out);

typedef void (*FlutterDataCallback)(const uint8_t* data , size_t size, void* user data);

void ForwardToHandler(FlutterMessenger* messenger,
                        const FlutterPlatformMessage* message,
                        void* user_data)
*/

/*
FLUTTER_EXPORT
FlutterEngineResult FlutterEngineSendPlatformMessageResponse(
    FLUTTER_API_SYMBOL(FlutterEngine) engine,
    const FlutterPlatformMessageResponseHandle* handle,
    const uint8_t* data,
    size_t data_length);
*/

void FlutterMessenger::Receive(const FlutterPlatformMessage &message)
{
    std::cout << "received message on:  " << message.channel << std::endl;

    auto *response_handle = message.response_handle;
    BinaryReply reply_handler = [this, response_handle](
                                    const uint8_t *reply,
                                    size_t reply_size) mutable
    {
        if (!response_handle)
        {
            std::cerr << "Error: Response can be set only once. Ignoring "
                            "duplicate response."
                        << std::endl;
            return;
        }
        FlutterEngineSendPlatformMessageResponse(engine_, response_handle, reply, reply_size);
    };

    if (channels_.find(message.channel) == channels_.end())
    {
        std::cout << "Channel does not exist: " << message.channel << std::endl;
        return;
    }
    channels_[message.channel]->HandleMessage(message, reply_handler);
    response_handle = nullptr;
}

void FlutterMessenger::AddChannel(Channel &channel)
{
    channels_[channel.name_] = &channel;
}

// ========== engine_method_result.h ==========

ReplyManager::ReplyManager(BinaryReply reply_handler)
    : reply_handler_(std::move(reply_handler))
{
    assert(reply_handler_);
}

ReplyManager::~ReplyManager()
{
    if (reply_handler_)
    {
        // Warn, rather than send a not-implemented response, since the engine may
        // no longer be valid at this point.
        std::cerr
            << "Warning: Failed to respond to a message. This is a memory leak."
            << std::endl;
    }
}

void ReplyManager::SendResponseData(const std::vector<uint8_t> *data)
{
    if (!reply_handler_)
    {
        std::cerr
            << "Error: Only one of Success, Error, or NotImplemented can be "
                "called,"
            << " and it can be called exactly once. Ignoring duplicate result."
            << std::endl;
        return;
    }

    const uint8_t *message = data && !data->empty() ? data->data() : nullptr;
    size_t message_size = data ? data->size() : 0;
    reply_handler_(message, message_size);
    reply_handler_ = nullptr;
}

} // namespace attacus