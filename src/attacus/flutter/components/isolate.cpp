#include <attacus/flutter/flutter_window.h>
#include <attacus/flutter/json_method_channel.h>

#include "isolate.h"

namespace attacus {

static constexpr char kChannelName[] = "flutter/isolate";

IsolateComponent::IsolateComponent(FlutterWindow &view) : FlutterComponent(view)
{
}

void IsolateComponent::Create()
{
    FlutterMessenger &messenger = flutter().messenger();
    //channel_ = new JsonMethodChannel(messenger, kChannelName);

    //TODO: https://api.flutter.dev/flutter/package-stream_channel_isolate_channel/IsolateChannel-class.html
}

}// namespace attacus