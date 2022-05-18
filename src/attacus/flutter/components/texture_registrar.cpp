#include <iostream>

#include <attacus/flutter/flutter_view.h>

#include "texture_registrar.h"

namespace attacus
{

    TextureRegistrar::TextureRegistrar(FlutterView& view) : FlutterComponent(view) {}

    TextureRegistrar::~TextureRegistrar() = default;

    int64_t TextureRegistrar::RegisterTexture(Texture& texture)
    {
        texture_id++;
        texture.id_ = texture_id;
        textures_[texture_id] = &texture;
        FlutterEngineResult result = FlutterEngineRegisterExternalTexture(engine_, texture_id);
        return texture_id;
    }

    bool TextureRegistrar::UnregisterTexture(int64_t texture_id)
    {
        FlutterEngineResult result = FlutterEngineUnregisterExternalTexture(engine_, texture_id);
        return true;
    }

    bool TextureRegistrar::MarkTextureFrameAvailable(int64_t texture_id)
    {
        FlutterEngineResult result = FlutterEngineMarkExternalTextureFrameAvailable(engine_, texture_id);
        return true;
    }

    bool TextureRegistrar::CopyTexture(int64_t texId, size_t width, size_t height, FlutterOpenGLTexture*  texOut) {
        return textures_[texId]->Copy(width, height, texOut);
    }

} // namespace attacus