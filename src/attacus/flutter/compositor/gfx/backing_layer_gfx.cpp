#include <iostream>

#include <bgfx/utils/utils.h>

#include "compositor_gfx.h"
#include "backing_surface_gfx.h"
#include "backing_layer_gfx.h"

namespace attacus {

struct PosTexcoordVertex {
    float x;
    float y;
    float z;
    float u;
    float v;
    float w;

    static void Init() {
        ms_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .end();
    }

    static bgfx::VertexLayout ms_layout;
};
bgfx::VertexLayout PosTexcoordVertex::ms_layout;

static PosTexcoordVertex s_quadVertices[4] = {
    {-1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f },
    { 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f },
    {-1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f },
    { 1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f }
};

static const uint16_t s_quadIndices[6] = {
    0, 1, 2,
    1, 3, 2
};

bool BackingLayerGfx::initialized_ = false;
bgfx::ProgramHandle BackingLayerGfx::program_;
bgfx::UniformHandle BackingLayerGfx::texture_color_;
bgfx::IndexBufferHandle BackingLayerGfx::ibh_;
bgfx::VertexBufferHandle BackingLayerGfx::vbh_;

void BackingLayerGfx::Create() {
    width_ = surface_->width();
    height_ = surface_->height();
    Init();
}

void BackingLayerGfx::Init() {
    if(initialized_) return;
    initialized_ = true;

    PosTexcoordVertex::Init();

    vbh_ = bgfx::createVertexBuffer(
        bgfx::makeRef(s_quadVertices, sizeof(s_quadVertices)),
        PosTexcoordVertex::ms_layout);

    ibh_ = bgfx::createIndexBuffer(
        bgfx::makeRef(s_quadIndices, sizeof(s_quadIndices)));

    program_ = loadProgram("vs_update", "fs_update_cmp");

    texture_color_ = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
}

void BackingLayerGfx::Destroy() {
    comp().FreeSurface(*surface_);
    Component::Destroy();
}

void BackingLayerGfx::Draw() {
        float scaleMat[16];
        float rotateMat[16];
        float transMat[16];

        float scale_ = 1.0f;
        bx::mtxScale(scaleMat, width_ * scale_, height_ * scale_, 1.0f);
        bx::mtxRotateXYZ(rotateMat, 0.0f, 0.0f, 0.0f);
        float x_ = 0.0f; float y_ = 0.0f;
        bx::mtxTranslate(transMat, x_, y_, 0.0f);

        float tmpMat[16];
        float tmpMat2[16];

        bx::mtxMul(tmpMat, scaleMat, rotateMat);
        bx::mtxMul(tmpMat2, tmpMat, transMat);

        bgfx::setTransform(tmpMat2);

        bgfx::setVertexBuffer(0, vbh_);
        bgfx::setIndexBuffer(ibh_);

        bgfx::setTexture(0, texture_color_, surface().texture_);

        uint64_t state = BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_ALPHA;

        bgfx::setState(state);

        bgfx::submit(0, program_);
}

} // namespace attacus