#pragma once

#include <attacus/shell/window_component.h>

#include <attacus/flutter/flutter_embedder.h>

namespace attacus {

class FlutterView;

class FlutterComponent : public WindowComponent<FlutterView> {
public:
    FlutterComponent(FlutterView& view);
    FlutterView& flutter() { return *window_; }
    virtual void Create() override;
    // Data members
    FlutterEngine engine_;
};

} //namespace attacus