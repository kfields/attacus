#pragma once

#include "compositor_layer_gfx.h"

namespace attacus {

class BackingSurfaceGfx;

class BackingLayerGfx : public CompositorLayerGfx {
public:
    BackingLayerGfx(CompositorGfx& comp, BackingSurfaceGfx& surface, FlutterPoint origin, FlutterSize size) : CompositorLayerGfx(comp, origin, size),
    surface_(&surface)
    {}
    void Create() override;
    void Init();
    void Destroy() override;
    void Draw() override;
    // Accesors
    BackingSurfaceGfx& surface() { return *surface_; }
    // Data members
    static bool initialized_;
    int width_;
    int height_;
    BackingSurfaceGfx* surface_ = nullptr;
    //
    static bgfx::ProgramHandle program_;
    static bgfx::UniformHandle texture_color_;
    static bgfx::IndexBufferHandle ibh_;
    static bgfx::VertexBufferHandle vbh_;
};

} //namespace attacus