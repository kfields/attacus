#pragma once

#include <attacus/flutter/flutter_embedder.h>
#include <attacus/core/component.h>

namespace attacus {

class CompositorGfx;

class CompositorLayerGfx : public Component {
public:
    CompositorLayerGfx(CompositorGfx& comp);
    CompositorLayerGfx(CompositorGfx& comp, FlutterPoint origin, FlutterSize size);
    virtual void Draw(){}
    // Accessors
    CompositorGfx& comp() { return *comp_; }
    // Data members
    CompositorGfx* comp_;
    FlutterPoint origin_;
    FlutterSize size_;
};

} //namespace attacus