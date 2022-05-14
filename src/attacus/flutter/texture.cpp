#include <glad/gl.h>

#include <bgfx/bgfx_p.h>
#include "texture.h"

/*
typedef struct {
  /// Target texture of the active texture unit (example GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE).
  uint32_t target;
  /// The name of the texture.
  uint32_t name;
  /// The texture format (example GL_RGBA8).
  uint32_t format;
  /// User data to be returned on the invocation of the destruction callback.
  void* user_data;
  /// Callback invoked (on an engine managed thread) that asks the embedder to
  /// collect the texture.
  VoidCallback destruction_callback;
  /// Optional parameters for texture height/width, default is 0, non-zero means
  /// the texture has the specified width/height. Usually, when the texture type
  /// is GL_TEXTURE_RECTANGLE, we need to specify the texture width/height to
  /// tell the embedder to scale when rendering.
  /// Width of the texture.
  size_t width;
  /// Height of the texture.
  size_t height;
} FlutterOpenGLTexture;
*/

namespace attacus
{

bool FrameBufferTexture::Copy(size_t width, size_t height, FlutterOpenGLTexture*  texOut) {
    bgfx::TextureHandle texIn = bgfx::getTexture(frameBuffer_);
    texOut->target = GL_TEXTURE_2D;
    texOut->name = bgfx::s_ctx->m_renderCtx->getInternal(texIn);
    texOut->width = width;
    texOut->height = height;
    texOut->format = GL_RGBA8;
    return true;
}

} // namespace attacus