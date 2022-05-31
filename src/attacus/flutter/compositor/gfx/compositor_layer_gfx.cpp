#include <iostream>

#include "compositor_gfx.h"

#include "compositor_layer_gfx.h"

namespace attacus {

CompositorLayerGfx::CompositorLayerGfx(CompositorGfx& comp) : comp_(&comp) {}

CompositorLayerGfx::CompositorLayerGfx(CompositorGfx& comp, FlutterPoint origin, FlutterSize size) :
comp_(&comp), origin_(origin), size_(size) {
    //std::cerr << "Layer: " << "x: " << origin.x << "y: " << origin.y << std::endl;
}

} // namespace attacus