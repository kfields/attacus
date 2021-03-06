// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <map>

#include "flutter_component.h"

namespace attacus
{
    class Surface;
    
    class TextureRegistrar : public FlutterComponent
    {
    public:
        explicit TextureRegistrar(FlutterView& view);
        virtual ~TextureRegistrar();

        // Prevent copying.
        TextureRegistrar(TextureRegistrar const &) = delete;
        TextureRegistrar &operator=(TextureRegistrar const &) = delete;

        // |flutter::TextureRegistrar|
        int64_t RegisterTexture(Surface& surface);
        // |flutter::TextureRegistrar|
        bool MarkTextureFrameAvailable(int64_t texture_id);

        // |flutter::TextureRegistrar|
        bool UnregisterTexture(int64_t texture_id);

        bool CopyTexture(int64_t texId, size_t width, size_t height, FlutterOpenGLTexture*  texOut);

        // Data members
        std::map<int64_t, Surface*> surfaces_;
    };

} // namespace attacus
