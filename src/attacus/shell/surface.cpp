#include <string>
#include <iostream>
#include <fmt/core.h>

#include <bgfx/bgfx_p.h>

#include "surface.h"

#include "view.h"

namespace attacus {

//TODO:Recycle handles
int16_t Surface::surface_count_ = 0;

Surface::Surface(SurfaceParams params) :
    size_(params.size),
    id_(surface_count_++)
{
}

Surface::~Surface() {
}

void Surface::SetSize(Size size) {
    size_ = size;
    OnSize();
}

uintptr_t Surface::GetInternal() {
    return bgfx::s_ctx->m_renderCtx->getInternal(GetTexture());
}

} //namespace attacus