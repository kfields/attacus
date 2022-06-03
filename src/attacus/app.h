#pragma once

#include <thread>

#include <attacus/shell/gfx_window.h>

namespace attacus {

class Gfx;

class App : public GfxWindow {
public:
    enum class State {
        kRunning,
        kShutdown
    };

    App(WindowParams params = WindowParams(), App* parent = nullptr);

    template<typename T = App>
    static T* Produce(WindowParams params = WindowParams()) {
        T* c = new T(params);
        c->Create();
        return c;
    }

    virtual ~App();
    void Create() override;
    void Destroy() override;
    bool Dispatch(SDL_Event& event) override;
    void Reset(ResetKind kind = ResetKind::kSoft) override;

    int Run() override;

    void RegisterResizer();
    void OnSize() override;
    // Accessors

    // Data members
    State state_;
    std::thread render_thread_;
    //
    Gfx* gfx_ = nullptr;
};

} // namespace attacus