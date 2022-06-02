#pragma once

#include "flutter_view.h"

namespace attacus {


class CompositorGL;

class FlutterCompositeViewGL : public FlutterView {
public:
    FlutterCompositeViewGL(View& parent, ViewParams params = ViewParams());
    void Create() override;
    void InitProjectArgs(FlutterProjectArgs& args) override;
    // Accessors
    CompositorGL& compositor() { return *compositor_; }
    // Data members
    CompositorGL* compositor_ = nullptr;
};

} //namespace attacus