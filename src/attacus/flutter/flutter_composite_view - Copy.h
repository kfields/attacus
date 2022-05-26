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

class FlutterCompositeView : public FlutterView {
public:
    FlutterCompositeView(View& parent, ViewParams params = ViewParams());
    void Create() override;
    void InitProjectArgs(FlutterProjectArgs& args) override;
    void InitCompositor(FlutterCompositor& compositor);
    //
    bool CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out);
    bool CollectBackingStore(const FlutterBackingStore& renderer);
    bool PresentLayers(const FlutterLayer** layers, size_t layers_count);
    void ScreenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft = false, float _width = 1.0f, float _height = 1.0f);

    void Draw() override;
    void DrawBackingStore(const FlutterLayer& layer);
    // Data members
    FlutterCompositor compositor_;
    //const FlutterLayer** layers_ = nullptr;
    //size_t layers_count_ = 0;
    std::vector<Layer> layers_;

    std::condition_variable cv_;
    std::mutex cv_m_;
    bool waiting_ = false;
    //
    std::mutex guard_mutex_;
    //
    bgfx::ProgramHandle program_;
    bgfx::UniformHandle uniform_;
    bgfx::VertexBufferHandle vbh_;
    bgfx::IndexBufferHandle ibh_;
};

} //namespace attacus