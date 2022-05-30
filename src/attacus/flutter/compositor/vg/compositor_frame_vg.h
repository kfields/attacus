#pragma once

#include "compositor_layer_vg.h"

namespace attacus {

class CompositorFrameVg : public CompositorLayerVg {
public:
    CompositorFrameVg(CompositorVg& comp) : CompositorLayerVg(comp) {}
    void Draw() override;
    void Destroy() override;
    void AddLayer(CompositorLayerVg* layer) { layers_.push_back(layer); }
    // Data members
    std::vector<CompositorLayerVg*> layers_;
};

} //namespace attacus