// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <map>

#include "surface.h"

namespace attacus
{

    class SurfaceManager
    {
    public:
        explicit SurfaceManager();
        virtual ~SurfaceManager();

        // Prevent copying.
        SurfaceManager(SurfaceManager const &) = delete;
        SurfaceManager &operator=(SurfaceManager const &) = delete;

        int16_t RegisterSurface(Surface& Surface);
        bool UnregisterSurface(int16_t surface_id);

        // Data members
        std::map<int16_t, Surface*> Surfaces_;
        // TODO: Recycle id's?
        int16_t surface_count = 0;
    };

} // namespace attacus
