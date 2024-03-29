#include <iostream>

#include <glad/gl.h>

#include <attacus/shell/surface.h>
#include <attacus/flutter/flutter_view.h>

#include "texture_registrar.h"

namespace attacus
{

    TextureRegistrar::TextureRegistrar(FlutterView& view) : FlutterComponent(view) {}

    TextureRegistrar::~TextureRegistrar() = default;

    int64_t TextureRegistrar::RegisterTexture(Surface& surface)
    {
        auto id = surface.id();
        surfaces_[id] = &surface;
        surface.touched_event_.Subscribe([this](SurfaceEvent e) {
            this->MarkTextureFrameAvailable(e.surface_->id());
        });
        FlutterEngineResult result = FlutterEngineRegisterExternalTexture(engine_, id);
        return id;
    }

    bool TextureRegistrar::UnregisterTexture(int64_t id)
    {
        FlutterEngineResult result = FlutterEngineUnregisterExternalTexture(engine_, id);
        return true;
    }

    bool TextureRegistrar::MarkTextureFrameAvailable(int64_t id)
    {
        //TODO: This used to work.  Now it's crashing when using the compositor
        //FlutterEngineResult result = FlutterEngineMarkExternalTextureFrameAvailable(engine_, id);
        FlutterEngineScheduleFrame(flutter().engine_);
        return true;
    }

    bool TextureRegistrar::CopyTexture(int64_t texId, size_t width, size_t height, FlutterOpenGLTexture*  texOut) {
        Surface& surface = *surfaces_[texId];
        texOut->target = GL_TEXTURE_2D;
        texOut->name = surface.GetTexture();
        texOut->width = width;
        texOut->height = height;
        texOut->format = GL_RGBA8;
        return true;
    }

} // namespace attacus