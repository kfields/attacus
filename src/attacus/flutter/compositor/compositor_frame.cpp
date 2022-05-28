#include <iostream>

#include "compositor.h"

#include "compositor_frame.h"

namespace attacus {

void CompositorFrame::Draw() {
    for (std::vector<CompositorLayer*>::iterator it = layers_.begin(); it != layers_.end(); ++it){
        CompositorLayer* layer = *it;
        layer->Draw();
    }
}
void CompositorFrame::Destroy() {
    for (std::vector<CompositorLayer*>::iterator it = layers_.begin(); it != layers_.end(); ++it) {
        CompositorLayer* layer = *it;
        layer->Destroy();
    }
}

} // namespace attacus