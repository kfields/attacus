#pragma once

#include "compositor_layer.h"

namespace attacus {

class BackingSurface;

class BackingLayer : public CompositorLayer {
public:
    BackingLayer(Compositor& comp, BackingSurface& surface, FlutterPoint origin, FlutterSize size) : CompositorLayer(comp, origin, size),
    surface_(&surface)
    {}
    void Create() override;
    void Destroy() override;
    void Draw() override;
    // Data members
    int width_;
    int height_;
    //vg::ImageHandle image_;
    vg::ImagePatternHandle imgPaint_;
    BackingSurface* surface_ = nullptr;
};

} //namespace attacus