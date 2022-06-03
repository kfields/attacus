#include <iostream>

#include <SDL.h>

#include "flutter_composite_view_gl.h"

#include "compositor/gl/compositor_gl.h"

namespace attacus
{

FlutterCompositeViewGL::FlutterCompositeViewGL(View& parent, ViewParams params) : FlutterView(parent, params)
{
    compositor_ = new CompositorGL(*this);
}

void FlutterCompositeViewGL::Create() {
    FlutterView::Create();
    compositor().Create();
}

void FlutterCompositeViewGL::InitProjectArgs(FlutterProjectArgs& args) {
    FlutterView::InitProjectArgs(args);
    args.compositor = compositor().InitCompositor();
}

} // namespace attacus