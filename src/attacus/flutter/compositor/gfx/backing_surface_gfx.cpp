#include <glad/gl.h>
#include "backing_surface_gfx.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <vg/vg.h>

#include "compositor_gfx.h"

namespace attacus {

BackingSurfaceGfx::BackingSurfaceGfx(CompositorGfx& comp, SurfaceParams params) : Surface(params){}

void BackingSurfaceGfx::Create() {
    Surface::Create();
    CreateTexture();
}

void BackingSurfaceGfx::Destroy() {
    Surface::Destroy();
}

} // namespace attacus