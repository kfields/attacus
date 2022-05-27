#pragma once

#include <condition_variable>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>

#include <attacus/flutter/components/flutter_component.h>

namespace vg {
struct Context;
}

namespace attacus {

class CompositorFrame;

class Compositor : public FlutterComponent {
public:
    Compositor(FlutterView& view);
    void Create() override;
    FlutterCompositor* InitCompositor();
    //
    bool DelegatedCreateBackingStore(const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out);
    bool CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out);
    
    bool DelegatedCollectBackingStore(const FlutterBackingStore* renderer);
    bool CollectBackingStore(const FlutterBackingStore& renderer);

    bool DelegatedPresentLayers(const FlutterLayer** layers, size_t layers_count);
    bool PresentLayers(const FlutterLayer** layers, size_t layers_count);

    virtual void Draw();
    // Accessors
    //std::queue<CompositorFrame*>& frames() { return frames_; }
    // Data members
    FlutterCompositor compositor_;
    std::queue<CompositorFrame*> frames_;
    CompositorFrame* frame_ = nullptr;
    //
    vg::Context* vg_ = nullptr;
    //
    std::mutex guard_mutex_;

    std::condition_variable cv_;
    std::mutex cv_m_;
    bool waiting_ = false;
};

} //namespace attacus