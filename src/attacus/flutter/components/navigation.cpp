#include <attacus/flutter/flutter_window.h>
#include <attacus/flutter/json_method_channel.h>

#include "navigation.h"

namespace attacus {

static constexpr char kChannelName[] = "flutter/navigation";

NavigationComponent::NavigationComponent(FlutterWindow &view) : FlutterComponent(view)
{
}

void NavigationComponent::Create()
{
    FlutterMessenger &messenger = flutter().messenger();
    channel_ = new JsonMethodChannel(messenger, kChannelName);
}

}// namespace attacus