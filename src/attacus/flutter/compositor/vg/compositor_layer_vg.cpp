#include <iostream>

#include "compositor_vg.h"

#include "compositor_layer_vg.h"

namespace attacus {

CompositorLayerVg::CompositorLayerVg(CompositorVg& comp) : comp_(&comp), vg_(comp.vg_) {}

CompositorLayerVg::CompositorLayerVg(CompositorVg& comp, FlutterPoint origin, FlutterSize size) :
comp_(&comp),vg_(comp.vg_), origin_(origin), size_(size) {
    //std::cerr << "Layer: " << "x: " << origin.x << "y: " << origin.y << std::endl;
}

} // namespace attacus