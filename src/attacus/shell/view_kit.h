// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <map>

#include "view.h"

namespace attacus
{

class ViewKit
{
public:
    explicit ViewKit();
    virtual ~ViewKit();

    // Prevent copying.
    ViewKit(ViewKit const &) = delete;
    ViewKit &operator=(ViewKit const &) = delete;

    int16_t RegisterView(View& View);
    bool UnregisterView(int16_t view_id);

    // Accessors
    ViewKit& instance() { return *instance_;}
    // Data members
    static ViewKit* instance_;
    std::map<int16_t, View*> views_;
    // TODO: Recycle id's?
    int16_t view_count = 0;
};

} // namespace attacus
