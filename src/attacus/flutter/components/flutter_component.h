#pragma once

#include <attacus/shell/window_component.h>

#include <attacus/flutter/flutter_embedder.h>

namespace attacus {

class FlutterWindow;

class FlutterComponent : public WindowComponent<FlutterWindow> {
public:
    FlutterComponent(FlutterWindow& view);
    FlutterWindow& flutter() { return *window_; }
    virtual void Create() override;
    // Data members
    FlutterEngine engine_;
};

} //namespace attacus