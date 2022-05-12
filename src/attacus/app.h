#pragma once

#include "flutter/flutter_view.h"

namespace attacus {

class App : public FlutterView {
public:
    enum class State {
        kRunning,
        kShutdown
    };

    App(WindowParams params = WindowParams());
    virtual ~App();
    virtual void Create() override;
    virtual void Destroy() override;
    virtual bool Dispatch(SDL_Event& event) override;
    virtual void PreRender() override;
    virtual void PostRender() override;
    virtual void Reset(ResetKind kind = ResetKind::kHard) override;

    int Run();

    void RegisterResizer();

    // Accessors
    // Data members
    State state_;
    uint32_t resetFlags_;
    uint32_t debugFlags_;
    int64_t timeOffset_;
    bool debug_ = false;
    bool capture_ = false;

};

} // namespace attacus