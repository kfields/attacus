#include <iostream>

#include <attacus/flutter/encodable_value.h>
#include <attacus/flutter/flutter_view.h>
#include <attacus/flutter/standard_method_channel.h>

#include "mouse_input.h"

namespace attacus {

MouseInput::MouseInput(FlutterView &view) : FlutterComponent(view)
{
}

void MouseInput::Create()
{
}

bool MouseInput::UpdatePointer(FlutterPointerPhase phase, double x, double y, size_t timestamp)
{
    FlutterPointerEvent event = {};
    event.struct_size = sizeof(event);
    event.device_kind = kFlutterPointerDeviceKindMouse;
    event.phase = phase;
    event.x = x * flutter().scaleFactor_;
    event.y = y * flutter().scaleFactor_;
    event.buttons = buttons_;
    event.timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::high_resolution_clock::now().time_since_epoch())
                            .count();
    FlutterEngineSendPointerEvent(flutter().engine_, &event, 1);
    return true;
}

bool MouseInput::Dispatch(SDL_Event &e)
{
    switch (e.type)
    {
    case SDL_WINDOWEVENT: {
        switch (e.window.event) {
        case SDL_WINDOWEVENT_ENTER:
            lastMouseX = e.motion.x;
            lastMouseY = e.motion.y;
            return UpdatePointer(FlutterPointerPhase::kAdd, e.motion.x, e.motion.y, e.motion.timestamp);
        case SDL_WINDOWEVENT_LEAVE:
            lastMouseX = e.motion.x;
            lastMouseY = e.motion.y;
            return UpdatePointer(FlutterPointerPhase::kRemove, e.motion.x, e.motion.y, e.motion.timestamp);
        }
    }
    case SDL_MOUSEMOTION: {
        if (mouseDown)
        {
            lastMouseX = e.motion.x;
            lastMouseY = e.motion.y;
            return UpdatePointer(FlutterPointerPhase::kMove, e.motion.x, e.motion.y, e.motion.timestamp);
        } else {
            lastMouseX = e.motion.x;
            lastMouseY = e.motion.y;
            return UpdatePointer(FlutterPointerPhase::kHover, e.motion.x, e.motion.y, e.motion.timestamp);
        }
        break;
    }
    case SDL_MOUSEBUTTONDOWN: {
        if (!mouseDown)
        {
            mouseDown = true;
            mouseId = e.button.which;
            buttons_ = buttons_ | (1 << e.button.which);
            lastMouseX = e.button.x;
            lastMouseY = e.button.y;
            return UpdatePointer(FlutterPointerPhase::kDown, e.button.x, e.button.y, e.button.timestamp);
        }
        break;
    }
    case SDL_MOUSEBUTTONUP: {
        if (mouseDown)
        {
            mouseDown = false;
            buttons_ = buttons_ & ~(1 << e.button.which);
            lastMouseX = e.button.x;
            lastMouseY = e.button.y;
            return UpdatePointer(FlutterPointerPhase::kUp, e.button.x, e.button.y, e.button.timestamp);
        }
        break;
    }
    }
}

} // namespace attacus