#pragma once

#include <condition_variable>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include "flutter_view.h"

namespace attacus {

typedef std::chrono::high_resolution_clock clock;
//typedef std::chrono::steady_clock clock;
typedef std::chrono::nanoseconds duration;
typedef duration::rep rep;
typedef duration::period period;
typedef std::chrono::time_point<clock, duration> time_point;

class CompositorVg;

class FlutterCompositeViewVg : public FlutterView {
public:
    FlutterCompositeViewVg(View& parent, ViewParams params = ViewParams());
    void Create() override;
    void InitProjectArgs(FlutterProjectArgs& args) override;
    //
    void PostRender() override;
    void Draw() override;
    //
    virtual void OnVSync(intptr_t baton);
    // Accessors
    CompositorVg& compositor() { return *compositor_; }
    // Data members
    CompositorVg* compositor_ = nullptr;
    std::chrono::nanoseconds start_time_ = std::chrono::nanoseconds::zero();
    std::chrono::nanoseconds last_time_ = std::chrono::nanoseconds::zero();;
    //intptr_t baton_;
    std::queue<intptr_t> batons_;
};

} //namespace attacus