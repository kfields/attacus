#include <iostream>

#include "compositor_gfx.h"

#include "compositor_frame_gfx.h"

namespace attacus {

void CompositorFrameGfx::Draw() {
    for (std::vector<CompositorLayerGfx*>::iterator it = layers_.begin(); it != layers_.end(); ++it){
        CompositorLayerGfx* layer = *it;
        layer->Draw();
    }
}
void CompositorFrameGfx::Destroy() {
    for (std::vector<CompositorLayerGfx*>::iterator it = layers_.begin(); it != layers_.end(); ++it) {
        CompositorLayerGfx* layer = *it;
        layer->Destroy();
    }
}

} // namespace attacus