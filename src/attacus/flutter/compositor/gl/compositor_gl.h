#pragma once

#include <list>

#include <attacus/flutter/compositor/compositor.h>

namespace attacus {

class BackingSurfaceGL;

class CompositorGL : public Compositor {
public:
    CompositorGL(FlutterView& view);
    FlutterCompositor* InitCompositor() override;
    //
    bool CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out);    
    bool CollectBackingStore(const FlutterBackingStore& renderer);
    bool PresentLayers(const FlutterLayer** layers, size_t layers_count);

    void PresentBackingStore(const FlutterBackingStore& store, FlutterPoint offset, FlutterSize size);
    void PresentPlatformView(const FlutterPlatformView& pview, FlutterPoint offset, FlutterSize size);
    void PresentTexture(uint32_t texId, FlutterPoint offset, FlutterSize size);

    BackingSurfaceGL* AllocSurface(FlutterSize size);
    void FreeSurface(BackingSurfaceGL& surface);
    BackingSurfaceGL* GetCachedSurface();
    // Accessors
    // Data members
    //FlutterCompositor compositor_;
    std::list<BackingSurfaceGL*> free_surfaces_;
};

} //namespace attacus