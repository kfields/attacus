#pragma once

#include <functional>

#include "SDL.h"

namespace attacus {

class Delegate {
public:
    Delegate() {}
    Delegate(std::function<void(void)> callback) : callback_(callback) {}
    void operator()() {
        callback_();
    }
    // Data members
    std::function<void(void)> callback_;
};

class Component {
public:
    virtual void Create() {}

    template<typename T = Component>
    static T& Produce() {
        T& c = *new T();
        c.Create();
        return c;
    }

    virtual bool Dispatch(SDL_Event& event);
    virtual bool DispatchWindowEvent(SDL_Event& event);
    virtual void OnResize(SDL_Event& event) {}
    virtual void OnSize() {}
    virtual void OnShow() {}

    static void PushCallbackEvent(Delegate* delegate, void* user_data);

};

} //namespace attacus
