#include <iostream>
#include <chrono>

#include <SDL.h>

#include <bx/timer.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <bgfx/utils/utils.h>

#include <vg/vg.h>

#include "flutter_composite_view_vg.h"

#include "compositor/vg/compositor_vg.h"

namespace attacus
{

FlutterCompositeViewVg::FlutterCompositeViewVg(View& parent, ViewParams params) : FlutterView(parent, params)
{
    view_id_ = 0;
    compositor_ = new CompositorVg(*this);
}

void FlutterCompositeViewVg::Create() {
    FlutterView::Create();

    //start_time_ = std::chrono::nanoseconds(engine_api_.GetCurrentTime());
    start_time_ = std::chrono::nanoseconds(bx::getHPCounter()*16);
    //last_time_ = std::chrono::nanoseconds(engine_api_.GetCurrentTime());
    last_time_ = std::chrono::nanoseconds(bx::getHPCounter()*16);

    bgfx::setViewMode(viewId(), bgfx::ViewMode::Sequential);

    /*bgfx::setViewClear(viewId()
        , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
        //, BGFX_CLEAR_NONE
        //, 0x303030ff
        , 0x12345678
        , 1.0f
        , 0
    );*/

    /*bgfx::setViewClear(viewId()
        , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH|BGFX_CLEAR_STENCIL
        , 0x303030ff
        , 1.0f
        , 0
        );*/

    bgfx::setViewRect(viewId(), 0, 0, width(), height());

    compositor().Create();
}

void FlutterCompositeViewVg::InitProjectArgs(FlutterProjectArgs& args) {
    FlutterView::InitProjectArgs(args);
    args.compositor = compositor().InitCompositor();
    args.vsync_callback = [](void* user_data, intptr_t baton)
    {
        FlutterCompositeViewVg* self = static_cast<FlutterCompositeViewVg*>(user_data);
        FlutterCompositeViewVg::PushCallbackEvent(new Delegate([self, baton]() -> void {
            self->OnVSync(baton);
        }), self);
        /*FlutterCompositeView::PushCallbackEvent(new Delegate([self, baton]() -> void {
            self->batons_.push(baton);
        }), self);*/
    };

}

// Lifted from vsync_waiter_fallback.cc
static std::chrono::nanoseconds SnapToNextTick(
    std::chrono::nanoseconds value,
    std::chrono::nanoseconds tick_phase,
    std::chrono::nanoseconds tick_interval) {
    std::chrono::nanoseconds offset = (tick_phase - value) % tick_interval;
    if (offset != std::chrono::nanoseconds::zero())
        offset = offset + tick_interval;
    return value + offset;
}

void FlutterCompositeViewVg::OnVSync(intptr_t baton) {
    //std::chrono::nanoseconds current_time = std::chrono::nanoseconds(engine_api_.GetCurrentTime());
    std::chrono::nanoseconds current_time = std::chrono::nanoseconds(bx::getHPCounter()*16);
    std::chrono::nanoseconds elapsed_time = current_time - last_time_;
    last_time_ = current_time;

    //std::chrono::nanoseconds frame_interval(elapsed_time);
    using namespace std::chrono_literals;
    std::chrono::nanoseconds frame_interval(16ms);
    auto next = SnapToNextTick(current_time, start_time_, frame_interval);
    FlutterEngineResult result = FlutterEngineOnVsync(engine_, baton, next.count(), (next + frame_interval).count());
    //TODO: Solve the vsync problem!!!
    //FlutterEngineResult result = FlutterEngineOnVsync(engine_, baton, 0, 0);  
}

void FlutterCompositeViewVg::PostRender() {
    FlutterView::PostRender();

    std::chrono::nanoseconds current_time = std::chrono::nanoseconds(engine_api_.GetCurrentTime());
    //std::chrono::nanoseconds current_time = std::chrono::nanoseconds(bx::getHPCounter());
    std::chrono::nanoseconds elapsed_time = current_time - last_time_;
    last_time_ = current_time;

    while (!batons_.empty()) {
        auto baton = batons_.front();
        batons_.pop();
        OnVSync(baton);
    }
}

void FlutterCompositeViewVg::Draw() {
    FlutterView::Draw();

    bgfx::touch(viewId());

    compositor().Draw();

    float ortho[16];
    const bgfx::Caps* caps = bgfx::getCaps();
    // Flip y
    bx::mtxOrtho(ortho, 0, width(), 0, height(), 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
    bgfx::setViewTransform(viewId(), NULL, ortho);


}

} // namespace attacus