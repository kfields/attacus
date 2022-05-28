#pragma once

#include <attacus/flutter/flutter_embedder.h>
#include <attacus/core/component.h>

namespace attacus {

class Compositor;

class CompositorLayer : public Component {
public:
    CompositorLayer(Compositor& comp);
    CompositorLayer(Compositor& comp, FlutterPoint origin, FlutterSize size);
    virtual void Draw(){}
    // Accessors
    Compositor& comp() { return *comp_; }
    // Data members
    Compositor* comp_;
    vg::Context* vg_;
    FlutterPoint origin_;
    FlutterSize size_;
};

} //namespace attacus