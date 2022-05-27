#include <iostream>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <bgfx/utils/utils.h>

#include <vg/vg.h>

#include "flutter_composite_view.h"

#include "compositor/compositor.h"

namespace attacus
{

FlutterCompositeView::FlutterCompositeView(View& parent, ViewParams params) : FlutterView(parent, params)
{
    view_id_ = 0;
    compositor_ = new Compositor(*this);
}

void FlutterCompositeView::Create() {
    FlutterView::Create();
    bgfx::setViewMode(viewId(), bgfx::ViewMode::Sequential);
    /*bgfx::setViewClear(viewId()
        , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
        //, 0x303030ff
        , 0x12345678
        , 1.0f
        , 0
    );*/

    bgfx::setViewRect(viewId(), 0, 0, width(), height());

    float ortho[16];
    const bgfx::Caps* caps = bgfx::getCaps();
    bx::mtxOrtho(ortho, 0, width(), height(), 0, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
    //bx::mtxOrtho(ortho, 0, width(), 0, height(), 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
    bgfx::setViewTransform(viewId(), NULL, ortho);

    compositor().Create();
}

void FlutterCompositeView::InitProjectArgs(FlutterProjectArgs& args) {
    FlutterView::InitProjectArgs(args);
    args.compositor = compositor().InitCompositor();
}

void FlutterCompositeView::Draw() {
    FlutterView::Draw();

    /*float ortho[16];
    const bgfx::Caps* caps = bgfx::getCaps();
    bx::mtxOrtho(ortho, 0, width(), height(), 0, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
    bgfx::setViewTransform(viewId(), NULL, ortho);*/


    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(viewId());
    compositor().Draw();
}

} // namespace attacus