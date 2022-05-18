#include <string>
#include <iostream>
#include <fmt/core.h>

#include <glad/gl.h>
#include "SDL.h"
#include "SDL_syswm.h"

#include <bx/timer.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "gfx_view.h"

namespace attacus {

void* GfxView::gfx_context_ = nullptr;
static bool initialized_ = false;
static void SetupBgfxPlatformData(const GfxView& app, bgfx::PlatformData &pd, const SDL_SysWMinfo &wmi);

GfxView::GfxView(View& parent, ViewParams params) : View(parent, params),
    resetFlags_(BGFX_RESET_VSYNC),
    debugFlags_(BGFX_DEBUG_TEXT),
    timeOffset_(bx::getHPCounter()) {
}

void GfxView::Create() {
    View::Create();
    CreateGfx();
}

void GfxView::PreRender() {
    View::PreRender();
    SDL_GL_MakeCurrent(sdl_window_, gfx_context_);
}

void GfxView::PostRender() {
    SDL_GL_MakeCurrent(sdl_window_, gfx_context_);
    bgfx::frame(capture_);
    View::PostRender();
    SDL_GL_MakeCurrent(sdl_window_, nullptr);
}

void GfxView::CreateGfx() {
    if (!gfx_context_) {
        gfx_context_ = CreateContext();
        if (gfx_context_ == NULL) {
            std::cout << fmt::format("Can't create opengl context for bgfx: {}\n", SDL_GetError());
            return;
        }
    }
    if (!initialized_) {
        InitGfx();
        initialized_ = true;
    }
}

void GfxView::InitGfx() {
    auto result = bgfx::renderFrame(); // single threaded mode

    bgfx::PlatformData pd{};
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(sdl_window_, &wmInfo);

    SetupBgfxPlatformData(*this, pd, wmInfo);

    bgfx::Init bgfx_init;
    bgfx_init.debug = debug_;
    bgfx_init.debug = true;
    //bgfx_init.type = bgfx::RendererType::Count; // auto choose renderer
    bgfx_init.type = bgfx::RendererType::OpenGL;
    bgfx_init.resolution.width = width();
    bgfx_init.resolution.height = height();
    bgfx_init.resolution.reset = resetFlags_;
    bgfx_init.platformData = pd;
    bgfx::init(bgfx_init);
    //SDL_GL_MakeCurrent(sdl_window_, nullptr);
}

void SetupBgfxPlatformData(const GfxView& app, bgfx::PlatformData &pd, const SDL_SysWMinfo &wmi) {
    switch (wmi.subsystem) {
        case SDL_SYSWM_UNKNOWN: abort();

#if defined(SDL_VIDEO_DRIVER_X11)
        case SDL_SYSWM_X11:
            pd.ndt = wmi.info.x11.display;
            pd.nwh = (void *)(uintptr_t)wmi.info.x11.window;
            break;
#endif

#if defined(SDL_VIDEO_DRIVER_WAYLAND)
        case SDL_SYSWM_WAYLAND:
            pd.ndt = wmi.info.wl.display;
            pd.nwh = wmi.info.wl.surface;
            break;
#endif

#if defined(SDL_VIDEO_DRIVER_MIR)
        case SDL_SYSWM_MIR:
            pd.ndt = wmi.info.mir.connection;
            pd.nwh = wmi.info.mir.surface;
            break;
#endif

#if defined(SDL_VIDEO_DRIVER_COCOA)
        case SDL_SYSWM_COCOA:
            pd.ndt = NULL;
            pd.nwh = wmi.info.cocoa.window;
            break;
#endif

#if defined(SDL_VIDEO_DRIVER_UIKIT)
        case SDL_SYSWM_UIKIT:
            pd.ndt = NULL;
            pd.nwh = wmi.info.uikit.window;
            break;
#endif

#if defined(SDL_VIDEO_DRIVER_WINDOWS)
        case SDL_SYSWM_WINDOWS:
            pd.ndt = NULL;
            pd.nwh = wmi.info.win.window;
            break;
#endif

#if defined(SDL_VIDEO_DRIVER_WINRT)
        case SDL_SYSWM_WINRT:
            pd.ndt = NULL;
            pd.nwh = wmi.info.winrt.window;
            break;
#endif

#if defined(SDL_VIDEO_DRIVER_VIVANTE)
        case SDL_SYSWM_VIVANTE:
            pd.ndt = wmi.info.vivante.display;
            pd.nwh = wmi.info.vivante.window;
            break;
#endif

        //default: spdlog::critical("Unknown Window system."); std::abort();
    }
    pd.context = app.gfx_context_;
    pd.nwh = NULL;
    pd.backBuffer = NULL;
    pd.backBufferDS = NULL;
}

} // namespace attacus