#pragma once

#include <functional>
#include "platform_window.h"

namespace attacus {

class Window : public PlatformWindow {
public:
    enum class State {
        kRunning,
        kShutdown
    };

    Window(Window& parent, WindowParams params = WindowParams());
    virtual ~Window();
    bool Dispatch(SDL_Event& event) override;
    //bool DispatchWindowEvent(SDL_Event& event) override;
    void OnShow() override;

    int Run() override;
    //Data members
    State state_;
    int64_t time_offset_;
    bool debug_ = false;
};

} //namespace attacus