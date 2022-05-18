// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include <bgfx/bgfx.h>
#include "flutter_embedder.h"

namespace attacus
{

    class Texture {
    public:
        virtual bool Copy(size_t width, size_t height, FlutterOpenGLTexture* texOut) { return false; }
        // Data members
        int64_t id_;
    };

    class FrameBufferTexture : public Texture
    {
    public:
        FrameBufferTexture(bgfx::FrameBufferHandle frameBuffer) : frameBuffer_(frameBuffer) {}
        bool Copy(size_t width, size_t height, FlutterOpenGLTexture*  texOut) override;
        // Data members
        bgfx::TextureHandle th_;
        uint32_t nh_; //native handle
        bgfx::FrameBufferHandle frameBuffer_;
    };

} // namespace attacus
