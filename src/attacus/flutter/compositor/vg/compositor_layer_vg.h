#pragma once

#include <attacus/flutter/flutter_embedder.h>
#include <attacus/core/component.h>

namespace attacus {

class CompositorVg;

class CompositorLayerVg : public Component {
public:
    CompositorLayerVg(CompositorVg& comp);
    CompositorLayerVg(CompositorVg& comp, FlutterPoint origin, FlutterSize size);
    virtual void Draw(){}
    // Accessors
    CompositorVg& comp() { return *comp_; }
    // Data members
    CompositorVg* comp_;
    vg::Context* vg_;
    FlutterPoint origin_;
    FlutterSize size_;
};

} //namespace attacus