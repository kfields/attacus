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

typedef std::function<View*(ViewParams)> ViewFactoryFn;

class ViewRegistry : public FlutterComponent
{
public:
    explicit ViewRegistry(FlutterView& view);
    virtual ~ViewRegistry();
    void Create() override;

    // Prevent copying.
    ViewRegistry(ViewRegistry const &) = delete;
    ViewRegistry &operator=(ViewRegistry const &) = delete;

    void RegisterView(int64_t id, View& view);
    bool UnregisterView(int64_t id);
    View* GetView(int64_t id) { return views_[id]; }
    void RegisterViewFactory(std::string name, ViewFactoryFn fn) {
        factories_[name] = fn;
    }
    // Accessors
    StandardMethodChannel& channel() { return *channel_; }
    // Data members
    StandardMethodChannel* channel_ = nullptr;

    // Data members
    std::map<int64_t, View*> views_;
    std::map<std::string, ViewFactoryFn> factories_;
};

} // namespace attacus
