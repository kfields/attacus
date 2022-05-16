#pragma once

#include <attacus/flutter/flutter_embedder.h>

#include "flutter_component.h"

namespace attacus {

class FlutterWindow;
class StandardMethodChannel;
class TextInputModel;

class MouseInput : FlutterComponent {
public:
    MouseInput(FlutterWindow& view);
    void Create();
    bool Dispatch(SDL_Event &e) override;
    bool UpdatePointer(FlutterPointerPhase phase, double x, double y, size_t timestamp);
    // Accessors
    // Data members
    bool mouseDown = false;
    int mouseId = 0;
    int lastMouseX = 0, lastMouseY = 0;
    int64_t buttons_ = 0;
};

} //namespace attacus