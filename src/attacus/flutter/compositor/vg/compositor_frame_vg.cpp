#include <iostream>

#include "compositor_vg.h"

#include "compositor_frame_vg.h"

namespace attacus {

void CompositorFrameVg::Draw() {
    for (std::vector<CompositorLayerVg*>::iterator it = layers_.begin(); it != layers_.end(); ++it){
        CompositorLayerVg* layer = *it;
        layer->Draw();
    }
}
void CompositorFrameVg::Destroy() {
    for (std::vector<CompositorLayerVg*>::iterator it = layers_.begin(); it != layers_.end(); ++it) {
        CompositorLayerVg* layer = *it;
        layer->Destroy();
    }
}

} // namespace attacus