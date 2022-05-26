#pragma once

#include <condition_variable>
#include <thread>
#include <mutex>
#include <vector>
#include "flutter_view.h"

namespace attacus {

class Layer {
    public:
    virtual void Draw(){}
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
    // Data members
    FlutterCompositor compositor_;
    std::vector<Layer*> layers_;
    //
    std::mutex guard_mutex_;
};

} //namespace attacus