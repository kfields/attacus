#include <iostream>

#include "compositor.h"

#include "compositor_layer.h"

namespace attacus {

CompositorLayer::CompositorLayer(Compositor& comp) : comp_(&comp), vg_(comp.vg_) {}

CompositorLayer::CompositorLayer(Compositor& comp, FlutterPoint origin, FlutterSize size) :
comp_(&comp),vg_(comp.vg_), origin_(origin), size_(size) {
    //std::cerr << "Layer: " << "x: " << origin.x << "y: " << origin.y << std::endl;
}

} // namespace attacus