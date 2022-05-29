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

/*void BackingSurface::Create() {
    Surface::Create();
    CreateTexture();
    glGenTextures(1, &texture_id);
    glGenFramebuffers(1, &framebuffer_id);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_RGBA,
                GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
}

void BackingSurface::Destroy() {
    glDeleteFramebuffers(1, &framebuffer_id);
    glDeleteTextures(1, &texture_id);
    Surface::Destroy();
}*/

/*void BackingSurface::Create() {
    Surface::Create();
    CreateTexture();
    glGenTextures(1, &texture_id);
    //bgfx::overrideInternal(texture_, texture_id);
    glGenFramebuffers(1, &framebuffer_id);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_RGBA,
                GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

    image_ = createImage(vg_, vg::ImageFlags::Filter_NearestUV, texture_);
}

void BackingSurface::Destroy() {
    glDeleteFramebuffers(1, &framebuffer_id);
    glDeleteTextures(1, &texture_id);

    vg::destroyImage(vg_, image_);

    Surface::Destroy();
}*/

} // namespace attacus