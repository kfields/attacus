#pragma once

#include <attacus/core/component.h>


typedef union SDL_Event SDL_Event;

namespace attacus {

class Dispatcher : public Component {
public:
    virtual bool Dispatch(SDL_Event& event);
    virtual bool DispatchWindowEvent(SDL_Event& event);
    virtual void OnResize(SDL_Event& event) {}
    virtual void OnSize() {}
    virtual void OnShow() {}

    static void PushCallbackEvent(Delegate* delegate, void* user_data);

};

} //namespace attacus
