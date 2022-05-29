#include <iostream>
#include <chrono>

#include <SDL.h>

#include <bx/timer.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <bgfx/utils/utils.h>

#include <vg/vg.h>

#include "flutter_composite_view.h"

#include "compositor/compositor.h"

namespace attacus
{

FlutterCompositeView::FlutterCompositeView(View& parent, ViewParams params) : FlutterView(parent, params)
{
    view_id_ = 0;
    compositor_ = new Compositor(*this);
    start_time_ = std::chrono::nanoseconds(bx::getHPCounter());
}

void FlutterCompositeView::Create() {
    FlutterView::Create();
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

void FlutterCompositeView::InitProjectArgs(FlutterProjectArgs& args) {
    FlutterView::InitProjectArgs(args);
    args.compositor = compositor().InitCompositor();
    args.vsync_callback = [](void* user_data, intptr_t baton)
    {
        FlutterCompositeView* self = static_cast<FlutterCompositeView*>(user_data);
        FlutterCompositeView::PushCallbackEvent(new Delegate([self, baton]() -> void {
            self->OnVSync(baton);
        }), self);
    };

}

/*
FlutterEngineResult FlutterEngineOnVsync(FLUTTER_API_SYMBOL(FlutterEngine)
                                             engine,
                                         intptr_t baton,
                                         uint64_t frame_start_time_nanos,
                                         uint64_t frame_target_time_nanos);

*/

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

void FlutterCompositeView::OnVSync(intptr_t baton) {
    //std::chrono::nanoseconds current_time = clock::now();
  std::chrono::nanoseconds current_time =
      std::chrono::nanoseconds(bx::getHPCounter());

    std::chrono::nanoseconds frame_interval(16600000);
    auto next = SnapToNextTick(current_time, start_time_, frame_interval);
    FlutterEngineResult result = FlutterEngineOnVsync(engine_, baton, next.count(), (next + frame_interval).count());    
}


/*
static int64_t NanosSinceEpoch(time_point tp) {
  const auto elapsed = tp.time_since_epoch();
  return std::chrono::duration_cast<duration>(elapsed).count();
}

void FlutterCompositeView::OnVSync(intptr_t baton) {
    time_point tp_start = clock::now();
    time_point tp_target = tp_start + std::chrono::milliseconds(17);
    uint64_t start = NanosSinceEpoch(tp_start);
    //uint64_t start = std::chrono::duration_cast<duration>(tp_start).count();
    uint64_t target = NanosSinceEpoch(tp_target);
    //uint64_t target = std::chrono::duration_cast<duration>(tp_target).count();
    FlutterEngineResult result = FlutterEngineOnVsync(engine_, baton, start, target);
}
*/
void FlutterCompositeView::Draw() {
    FlutterView::Draw();

    compositor().Draw();
    bgfx::touch(viewId());

    float ortho[16];
    const bgfx::Caps* caps = bgfx::getCaps();
    // Flip y
    bx::mtxOrtho(ortho, 0, width(), 0, height(), 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
    bgfx::setViewTransform(viewId(), NULL, ortho);

}

} // namespace attacus