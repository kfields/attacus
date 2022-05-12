// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "flutter_component.h"

#include "texture.h"

namespace attacus
{

    class TextureRegistrar : public FlutterComponent
    {
    public:
        explicit TextureRegistrar(FlutterView& view);
        virtual ~TextureRegistrar();

        // Prevent copying.
        TextureRegistrar(TextureRegistrar const &) = delete;
        TextureRegistrar &operator=(TextureRegistrar const &) = delete;

        // |flutter::TextureRegistrar|
        int64_t RegisterTexture(TextureVariant *texture);
        int64_t InternalRegisterTexture(const FlutterDesktopTextureInfo *texture_info);
        // |flutter::TextureRegistrar|
        bool MarkTextureFrameAvailable(int64_t texture_id);

        // |flutter::TextureRegistrar|
        bool UnregisterTexture(int64_t texture_id);

        // Data members
        // TODO: Recycle id's?
        int64_t texture_id = 0;
    };

} // namespace attacus
