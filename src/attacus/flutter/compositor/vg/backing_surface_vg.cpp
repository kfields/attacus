#include <glad/gl.h>
#include "backing_surface_vg.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <vg/vg.h>

#include "compositor_vg.h"

namespace attacus {

BackingSurfaceVg::BackingSurfaceVg(CompositorVg& comp, SurfaceParams params) : Surface(params), vg_(comp.vg_){}

void BackingSurfaceVg::Create() {
    Surface::Create();
    CreateTexture();
    image_ = createImage(vg_, vg::ImageFlags::Filter_NearestUV, texture_);
}

void BackingSurfaceVg::Destroy() {
    vg::destroyImage(vg_, image_);
    Surface::Destroy();
}

} // namespace attacus