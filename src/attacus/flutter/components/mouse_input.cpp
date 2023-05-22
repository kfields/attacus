#include <iostream>

#include <attacus/flutter/encodable_value.h>
#include <attacus/flutter/flutter_view.h>
#include <attacus/flutter/standard_method_channel.h>

#include "mouse_input.h"

namespace attacus
{

    MouseInput::MouseInput(FlutterView &view) : FlutterComponent(view)
    {
    }

    void MouseInput::Create()
    {
    }

    /*bool MouseInput::UpdatePointer(FlutterPointerPhase phase, double x, double y, size_t timestamp)
    {
      FlutterPointerEvent event = {};
      event.struct_size = sizeof(event);
      event.device_kind = kFlutterPointerDeviceKindMouse;
      event.phase = phase;
      event.x = x * flutter().scaleFactor_;
      event.y = y * flutter().scaleFactor_;
      event.buttons = buttons_;
      event.timestamp = timestamp;

      FlutterEngineSendPointerEvent(flutter().engine_, &event, 1);
      return true;
    }*/

    bool MouseInput::UpdatePointer(FlutterPointerPhase phase, size_t timestamp, float x, float y, float scroll_delta_x, float scroll_delta_y)
    {
        FlutterPointerEvent event = {};
        event.struct_size = sizeof(event);
        event.device_kind = kFlutterPointerDeviceKindMouse;
        event.phase = phase;
        auto pixelRatio = flutter().pixelRatio_;
        event.x = x * pixelRatio;
        event.y = y * pixelRatio;
        if (scroll_delta_x != 0 || scroll_delta_y != 0) {
            event.signal_kind = kFlutterPointerSignalKindScroll;
        }
        event.scroll_delta_x = scroll_delta_x * pixelRatio;
        event.scroll_delta_y = scroll_delta_y * pixelRatio;
        event.buttons = buttons_;
        event.timestamp = timestamp;

        FlutterEngineSendPointerEvent(flutter().engine_, &event, 1);
        return true;
    }

    bool MouseInput::Dispatch(SDL_Event &e)
    {
        switch (e.type)
        {
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
            entered_ = true;
            return true;

        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            entered_ = false;
            return true;

        case SDL_EVENT_MOUSE_WHEEL:
        {
            float dx = e.wheel.x;
            float dy = -e.wheel.y * 4; //TODO: mouse wheel sensitivity?
            UpdatePointer(FlutterPointerPhase::kDown, e.motion.timestamp, e.wheel.mouseX, e.wheel.mouseY, dx, dy);
            UpdatePointer(FlutterPointerPhase::kMove, e.motion.timestamp, e.wheel.mouseX, e.wheel.mouseY, dx, dy);
            UpdatePointer(FlutterPointerPhase::kUp, e.motion.timestamp, e.wheel.mouseX, e.wheel.mouseY, dx, dy);
        }
        case SDL_EVENT_MOUSE_MOTION:
        {
            if (!entered_)
                break;
            if (mouseDown)
            {
                lastMouseX = e.motion.x;
                lastMouseY = e.motion.y;
                return UpdatePointer(FlutterPointerPhase::kMove, e.motion.timestamp, e.motion.x, e.motion.y);
            }
            else
            {
                lastMouseX = e.motion.x;
                lastMouseY = e.motion.y;
                return UpdatePointer(FlutterPointerPhase::kHover, e.motion.timestamp, e.motion.x, e.motion.y);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        {
            if (!entered_)
                break;
            if (!mouseDown)
            {
                mouseDown = true;
                mouseId = e.button.which;
                buttons_ = buttons_ | (static_cast<int64_t>(1) << e.button.which);
                lastMouseX = e.button.x;
                lastMouseY = e.button.y;
                return UpdatePointer(FlutterPointerPhase::kDown, e.button.timestamp, e.button.x, e.button.y);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP:
        {
            if (!entered_)
                break;
            if (mouseDown)
            {
                mouseDown = false;
                buttons_ = buttons_ & ~(1 << e.button.which);
                lastMouseX = e.button.x;
                lastMouseY = e.button.y;
                return UpdatePointer(FlutterPointerPhase::kUp, e.button.timestamp, e.button.x, e.button.y);
            }
            break;
        }
        }
        return true;
    }

} // namespace attacus