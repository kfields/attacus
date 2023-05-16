#pragma once

#include <attacus/flutter/flutter_embedder.h>

#include "flutter_component.h"

namespace attacus {

class FlutterView;
class StandardMethodChannel;
class TextInputModel;

class MouseInput : FlutterComponent {
public:
    MouseInput(FlutterView& view);
    void Create();
    bool Dispatch(SDL_Event &e) override;
    bool UpdatePointer(FlutterPointerPhase phase, float x, float y, size_t timestamp);
    // Accessors
    // Data members
    bool entered_ = false;
    bool mouseDown = false;
    int mouseId = 0;
    float lastMouseX = 0.0f, lastMouseY = 0.0f;
    int64_t buttons_ = 0;
};

} //namespace attacus