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
    void Create() override;
    void Destroy() override;
    bool Dispatch(SDL_Event& event) override;
    void Reset(ResetKind kind = ResetKind::kSoft) override;

    int Run() override;
    //Data members
    State state_;
    int64_t time_offset_;
    bool debug_ = false;
};

} //namespace attacus