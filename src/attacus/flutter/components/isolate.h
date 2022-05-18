#pragma once

#include "flutter_component.h"

namespace attacus {

class FlutterView;
class JsonMethodChannel;
class TextInputModel;

class IsolateComponent : FlutterComponent {
public:
    IsolateComponent(FlutterView& view);
    void Create();
    // Accessors
    JsonMethodChannel& channel() { return *channel_; }
    // Data members
    JsonMethodChannel* channel_ = nullptr;
};

} //namespace attacus