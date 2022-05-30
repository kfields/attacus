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

class CompositorFrame;
class BackingSurface;

class Compositor : public FlutterComponent {
public:
    Compositor(FlutterView& view);
    void Create() override;
    FlutterCompositor* InitCompositor();
    //
    bool DelegatedCreateBackingStore(const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out);
    bool CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out, BackingSurface& surface);
    
    bool DelegatedCollectBackingStore(const FlutterBackingStore* renderer);
    bool CollectBackingStore(const FlutterBackingStore& renderer);

    bool DelegatedPresentLayers(const FlutterLayer** layers, size_t layers_count);
    bool PresentLayers(const FlutterLayer** layers, size_t layers_count);

    virtual void Draw();

    BackingSurface* AllocSurface(FlutterSize size);
    void FreeSurface(BackingSurface& surface);
    BackingSurface* GetCachedSurface();
    // Accessors
    // Data members
    FlutterCompositor compositor_;
    std::queue<CompositorFrame*> frames_;
    std::list<BackingSurface*> free_surfaces_;
    CompositorFrame* frame_ = nullptr;
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