// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <map>

#include "flutter_component.h"

namespace attacus
{

class View;
class StandardMethodChannel;

class ViewRegistrar : public FlutterComponent
{
public:
    explicit ViewRegistrar(FlutterView& view);
    virtual ~ViewRegistrar();
    void Create() override;

    // Prevent copying.
    ViewRegistrar(ViewRegistrar const &) = delete;
    ViewRegistrar &operator=(ViewRegistrar const &) = delete;

    int64_t RegisterView(View& view);
    bool UnregisterView(int64_t view_id);
    // Accessors
    StandardMethodChannel& channel() { return *channel_; }
    // Data members
    StandardMethodChannel* channel_ = nullptr;

    // Data members
    std::map<int64_t, View*> views_;
};

} // namespace attacus
