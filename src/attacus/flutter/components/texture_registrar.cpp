#include <iostream>

#include "texture_registrar.h"

namespace attacus
{

    TextureRegistrar::TextureRegistrar(FlutterView& view) : FlutterComponent(view) {}

    TextureRegistrar::~TextureRegistrar() = default;

    int64_t TextureRegistrar::RegisterTexture(TextureVariant *texture)
    {
        if (auto pixel_buffer_texture = std::get_if<PixelBufferTexture>(texture))
        {
            FlutterDesktopTextureInfo info = {};
            info.type = kFlutterDesktopPixelBufferTexture;
            info.pixel_buffer_config.user_data = pixel_buffer_texture;
            info.pixel_buffer_config.callback =
                [](size_t width, size_t height,
                   void *user_data) -> const FlutterDesktopPixelBuffer *
            {
                auto texture = static_cast<PixelBufferTexture *>(user_data);
                auto buffer = texture->CopyPixelBuffer(width, height);
                return buffer;
            };
            //int64_t texture_id = FlutterDesktopTextureRegistrarRegisterExternalTexture(texture_registrar_ref_, &info);
            FlutterEngineResult result = FlutterEngineRegisterExternalTexture(engine_, ++texture_id);
            return texture_id;
        }

        std::cerr << "Attempting to register unknown texture variant." << std::endl;
        return -1;
    } // namespace flutter

    bool TextureRegistrar::UnregisterTexture(int64_t texture_id)
    {
        //return FlutterDesktopTextureRegistrarUnregisterExternalTexture(texture_registrar_ref_, texture_id);
        FlutterEngineResult result = FlutterEngineUnregisterExternalTexture(engine_, texture_id);
        return true;
    }

    bool TextureRegistrar::MarkTextureFrameAvailable(int64_t texture_id)
    {
        //return FlutterDesktopTextureRegistrarMarkExternalTextureFrameAvailable(texture_registrar_ref_, texture_id);
        FlutterEngineResult result = FlutterEngineMarkExternalTextureFrameAvailable(engine_, texture_id);
        return true;
    }

} // namespace attacus