#pragma once

#include <condition_variable>
#include <thread>
#include <mutex>
#include <vector>
#include "flutter_view.h"

namespace attacus {

class Compositor;

class FlutterCompositeView : public FlutterView {
public:
    FlutterCompositeView(View& parent, ViewParams params = ViewParams());
    void Create() override;
    void InitProjectArgs(FlutterProjectArgs& args) override;
    //
    void Draw() override;
    // Accessors
    Compositor& compositor() { return *compositor_; }
    // Data members
    Compositor* compositor_ = nullptr;
};

} //namespace attacus