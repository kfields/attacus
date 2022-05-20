// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <map>

#include "surface.h"

namespace attacus
{

class SurfaceKit
{
public:
    explicit SurfaceKit();
    virtual ~SurfaceKit();

    // Prevent copying.
    SurfaceKit(SurfaceKit const &) = delete;
    SurfaceKit &operator=(SurfaceKit const &) = delete;

    int16_t RegisterSurface(Surface& Surface);
    bool UnregisterSurface(int16_t view_id);

    // Accessors
    SurfaceKit& instance() { return *instance_;}
    // Data members
    static SurfaceKit* instance_;
    std::map<int16_t, Surface*> views_;
    // TODO: Recycle id's?
    int16_t view_count = 0;
};

} // namespace attacus
