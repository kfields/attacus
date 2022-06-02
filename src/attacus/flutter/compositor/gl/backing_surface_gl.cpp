#include <glad/gl.h>
#include "backing_surface_gl.h"

namespace attacus {

void BackingSurfaceGL::Create() {
    Surface::Create();
    glGenTextures(1, &texture_id_);
    glGenFramebuffers(1, &framebuffer_id_);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id_);

    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_RGBA,
                GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id_, 0);
    
}

void BackingSurfaceGL::Destroy() {
    glDeleteFramebuffers(1, &framebuffer_id_);
    glDeleteTextures(1, &texture_id_);
    Surface::Destroy();
}

} // namespace attacus