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
        //, BGFX_CLEAR_NONE
        //, 0x303030ff
        , 0x12345678
        , 1.0f
        , 0
    );*/

    /*bgfx::setViewClear(viewId()
        , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH|BGFX_CLEAR_STENCIL
        , 0x303030ff
        , 1.0f
        , 0
        );*/

    bgfx::setViewRect(viewId(), 0, 0, width(), height());

    compositor().Create();
}

void FlutterCompositeView::InitProjectArgs(FlutterProjectArgs& args) {
    FlutterView::InitProjectArgs(args);
    args.compositor = compositor().InitCompositor();
}

void FlutterCompositeView::Draw() {
    FlutterView::Draw();

    compositor().Draw();
    bgfx::touch(viewId());

    float ortho[16];
    const bgfx::Caps* caps = bgfx::getCaps();
    // Flip y
    bx::mtxOrtho(ortho, 0, width(), 0, height(), 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
    bgfx::setViewTransform(viewId(), NULL, ortho);

}

} // namespace attacus