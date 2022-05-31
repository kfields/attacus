#pragma once

#include <condition_variable>
#include <thread>
#include <mutex>
#include <semaphore>
#include <list>
#include <queue>

#include <attacus/flutter/components/flutter_component.h>

namespace vg {
struct Context;
}

namespace attacus {

class CompositorFrameGfx;
class BackingSurfaceGfx;

class CompositorGfx : public FlutterComponent {
public:
    CompositorGfx(FlutterView& view);
    void Create() override;
    FlutterCompositor* InitCompositor();
    //
    bool DelegatedCreateBackingStore(const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out);
    bool CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out, BackingSurfaceGfx& surface);
    
    bool DelegatedCollectBackingStore(const FlutterBackingStore* renderer);
    bool CollectBackingStore(const FlutterBackingStore& renderer);

    bool DelegatedPresentLayers(const FlutterLayer** layers, size_t layers_count);
    bool PresentLayers(const FlutterLayer** layers, size_t layers_count);

    virtual void Draw();

    BackingSurfaceGfx* AllocSurface(FlutterSize size);
    void FreeSurface(BackingSurfaceGfx& surface);
    BackingSurfaceGfx* GetCachedSurface();
    // Accessors
    // Data members
    FlutterCompositor compositor_;
    std::queue<CompositorFrameGfx*> frames_;
    std::list<BackingSurfaceGfx*> free_surfaces_;
    CompositorFrameGfx* frame_ = nullptr;
    //
    vg::Context* vg_ = nullptr;
    //
    std::mutex guard_mutex_;
    std::condition_variable cv_;
    std::mutex cv_m_;
    bool waiting_ = false;
    //
    std::mutex render_mutex_;
    std::condition_variable render_cv_;
    std::mutex render_cv_m_;
    bool rendering_ = false;
    //
    std::chrono::nanoseconds throttle_ = std::chrono::nanoseconds::zero();
};

} //namespace attacus