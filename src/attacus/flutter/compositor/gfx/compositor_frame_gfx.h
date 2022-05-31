#pragma once

#include "compositor_layer_gfx.h"

namespace attacus {

class CompositorFrameGfx : public CompositorLayerGfx {
public:
    CompositorFrameGfx(CompositorGfx& comp) : CompositorLayerGfx(comp) {}
    void Draw() override;
    void Destroy() override;
    void AddLayer(CompositorLayerGfx* layer) { layers_.push_back(layer); }
    // Data members
    std::vector<CompositorLayerGfx*> layers_;
};

} //namespace attacus