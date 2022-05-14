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

    Window(WindowParams params = WindowParams());
    virtual ~Window();
    virtual void Create() override;
    virtual void Destroy() override;
    virtual bool Dispatch(SDL_Event& event) override;
    virtual void Reset(ResetKind kind = ResetKind::kHard) override;

    int Run();
    virtual void Startup() {};
    virtual void Shutdown() {};
    //Data members
    State state_;
    uint32_t resetFlags_;
    uint32_t debugFlags_;
    int64_t timeOffset_;
    bool debug_ = false;
    bool capture_ = false;

};

} //namespace attacus