#include <attacus/flutter/flutter_view.h>
#include <attacus/flutter/json_method_channel.h>

#include "platform.h"

namespace attacus {

static constexpr char kChannelName[] = "flutter/platform";

PlatformComponent::PlatformComponent(FlutterView &view) : FlutterComponent(view)
{
}

void PlatformComponent::Create()
{
    FlutterMessenger &messenger = flutter().messenger();
    channel_ = new JsonMethodChannel(messenger, kChannelName);
}

}// namespace attacus