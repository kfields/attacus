#include <glad/gl.h>
#include "backing_surface.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <vg/vg.h>

#include "compositor.h"

namespace attacus {

BackingSurface::BackingSurface(Compositor& comp, SurfaceParams params) : Surface(params), vg_(comp.vg_){}

void BackingSurface::Create() {
    Surface::Create();
    CreateTexture();
    image_ = createImage(vg_, vg::ImageFlags::Filter_NearestUV, texture_);
    //imgPaint_ = createImagePattern(vg_, 0, 0, width(), height(), 0.0f, image_);
}

void BackingSurface::Destroy() {
    vg::destroyImage(vg_, image_);
    Surface::Destroy();
}

} // namespace attacus