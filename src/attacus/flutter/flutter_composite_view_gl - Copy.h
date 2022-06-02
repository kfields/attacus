#pragma once

#include <condition_variable>
#include <thread>
#include <mutex>
#include <vector>
#include "flutter_view.h"

namespace attacus {

class Layer {
    public:
};

class TextureLayer : public Layer {
public:
    TextureLayer(uint32_t texture_id) : texture_id_(texture_id) {}
    // Data members
    uint32_t texture_id_;
};

class FlutterCompositeViewGL : public FlutterView {
public:
    FlutterCompositeViewGL(View& parent, ViewParams params = ViewParams());
    void Create() override;
    void InitProjectArgs(FlutterProjectArgs& args) override;
    void InitCompositor(FlutterCompositor& compositor);
    //
    bool CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out);
    bool CollectBackingStore(const FlutterBackingStore& renderer);
    bool PresentLayers(const FlutterLayer** layers, size_t layers_count);

    void PresentBackingStore(const FlutterBackingStore& store, FlutterPoint offset, FlutterSize size);
    void PresentPlatformView(const FlutterPlatformView& pview, FlutterPoint offset, FlutterSize size);
    void PresentTexture(uint32_t texId, FlutterPoint offset, FlutterSize size);
    // Data members
    FlutterCompositor compositor_;
};

} //namespace attacus