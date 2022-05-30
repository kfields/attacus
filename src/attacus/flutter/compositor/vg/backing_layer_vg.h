#pragma once

#include "compositor_layer_vg.h"

namespace attacus {

class BackingSurfaceVg;

class BackingLayerVg : public CompositorLayerVg {
public:
    BackingLayerVg(CompositorVg& comp, BackingSurfaceVg& surface, FlutterPoint origin, FlutterSize size) : CompositorLayerVg(comp, origin, size),
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
    BackingSurfaceVg* surface_ = nullptr;
};

} //namespace attacus