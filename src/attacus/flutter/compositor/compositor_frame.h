#pragma once

#include "compositor_layer.h"

namespace attacus {

class CompositorFrame : public CompositorLayer {
public:
    CompositorFrame(Compositor& comp) : CompositorLayer(comp) {}
    void Draw() override;
    void Destroy() override;
    void AddLayer(CompositorLayer* layer) { layers_.push_back(layer); }
    // Data members
    std::vector<CompositorLayer*> layers_;
};

} //namespace attacus