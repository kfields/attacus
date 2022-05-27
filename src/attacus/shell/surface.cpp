#include <string>
#include <iostream>
#include <fmt/core.h>

#include <bgfx/bgfx.h>
#include <bgfx/bgfx_p.h>

#include "surface.h"

#include "view.h"

namespace attacus {

//TODO:Recycle handles
int16_t Surface::surface_count_ = 0;

Surface::Surface(SurfaceParams params) :
    size_(params.size),
    id_(surface_count_++),
    texture_(BGFX_INVALID_HANDLE),
    frameBuffer_(BGFX_INVALID_HANDLE)
{
}

Surface::~Surface() {
}

void Surface::Destroy() {
    if (bgfx::isValid(texture_)) {
        bgfx::destroy(texture_);
    }
    if (bgfx::isValid(frameBuffer_)) {
        bgfx::destroy(frameBuffer_);
    }
}

void Surface::CreateTexture() {
    /*const uint64_t tsFlags = 0
        | BGFX_SAMPLER_MIN_POINT
        | BGFX_SAMPLER_MAG_POINT
        | BGFX_SAMPLER_MIP_POINT
        | BGFX_SAMPLER_U_CLAMP
        | BGFX_SAMPLER_V_CLAMP
        ;*/
    //const uint64_t tsFlags = BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;
    //const uint64_t tsFlags = BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT;
    const uint64_t tsFlags = BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_TEXTURE_RT | BGFX_TEXTURE_SRGB;
    texture_ = bgfx::createTexture2D(width(), height(), false, 1, bgfx::TextureFormat::RGBA8, tsFlags);
}

void Surface::CreateFramebuffer() {
    const uint64_t tsFlags = 0
        | BGFX_SAMPLER_MIN_POINT
        | BGFX_SAMPLER_MAG_POINT
        | BGFX_SAMPLER_MIP_POINT
        | BGFX_SAMPLER_U_CLAMP
        | BGFX_SAMPLER_V_CLAMP
        ;
    frameBuffer_ = bgfx::createFrameBuffer(width(), height(), bgfx::TextureFormat::RGBA8, tsFlags);
}

void Surface::SetSize(Size size) {
    size_ = size;
    OnSize();
}

uintptr_t Surface::GetInternalTexture() {
    return bgfx::s_ctx->m_renderCtx->getInternal(GetTexture());
}

} //namespace attacus