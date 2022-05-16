#include <string>
#include <iostream>

#include "SDL.h"
#include "SDL_syswm.h"

#include <bx/math.h>
#include <bx/timer.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "app.h"
#include <attacus/flutter/flutter_window.h>

namespace attacus {

static void SetupBgfxPlatformData(const App& app, bgfx::PlatformData &pd, const SDL_SysWMinfo &wmi);

App::App(WindowParams params) : FlutterWindow(params),
    state_(State::kRunning),
    resetFlags_(BGFX_RESET_VSYNC),
    debugFlags_(BGFX_DEBUG_TEXT),
    timeOffset_(bx::getHPCounter())
    {}

App::~App() {
}

void App::Create() {
    FlutterWindow::Create();
}

void App::CreateGfx() {
    SDL_GL_MakeCurrent(sdl_window_, gfx_context_);
    auto result = bgfx::renderFrame(); // single threaded mode

    bgfx::PlatformData pd{};
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(sdl_window_, &wmInfo);

    SetupBgfxPlatformData(*this, pd, wmInfo);

    bgfx::Init bgfx_init;
    //bgfx_init.debug = debug_;
    bgfx_init.debug = true;
    //bgfx_init.type = bgfx::RendererType::Count; // auto choose renderer
    bgfx_init.type = bgfx::RendererType::OpenGL;
    bgfx_init.resolution.width = width();
    bgfx_init.resolution.height = height();
    bgfx_init.resolution.reset = resetFlags_;
    bgfx_init.platformData = pd;
    bgfx::init(bgfx_init);
    SDL_GL_MakeCurrent(sdl_window_, nullptr);
}

void App::Destroy() {
    FlutterWindow::Destroy();
}

void App::Reset(ResetKind kind)
{
    //bgfx::reset(width(), height(), resetFlags_);
    //bgfx::setViewRect(viewId(), 0, 0, width(), height());
    for (auto child : children_) {
        child->Reset(ResetKind::kSoft);
    }
}

bool App::Dispatch(SDL_Event& event) {
    if (state_ == State::kShutdown)
        return false;

    if (event.window.windowID != windowId()) {
        WindowBase* child = windowMap_[event.window.windowID];
        if (child != nullptr)
            return windowMap_[event.window.windowID]->Dispatch(event);
    }

    capture_ = false;
    switch (event.type)
    {
        case SDL_USEREVENT:
        {
            Delegate& delegate = *static_cast<Delegate*>(event.user.data1);
            delegate();
            delete &delegate;
            event.user.data1 = nullptr;
            break;
        }
        case SDL_KEYDOWN:
        {
            int key = event.key.keysym.scancode;
            if (key == SDL_SCANCODE_F11) {
                capture_ = true;
                return true;
            }
        }
    }
    return FlutterWindow::Dispatch(event);
}

void App::PreRender() {
    FlutterWindow::PreRender();
    SDL_GL_MakeCurrent(sdl_window_, gfx_context_);
}

void App::PostRender() {
    bgfx::frame(capture_);
    FlutterWindow::PostRender();
    SDL_GL_MakeCurrent(sdl_window_, nullptr);
}

int App::Run() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (SDL_GL_LoadLibrary(nullptr) < 0) {
        printf("SDL_GL_LoadLibrary failed. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    RegisterResizer();

    FlutterWindow::Run();

    SDL_Quit();

    return 0;
}

static int resizingEventWatcher(void* data, SDL_Event* event) {
    if (event->type == SDL_WINDOWEVENT &&
        event->window.event == SDL_WINDOWEVENT_RESIZED) {
        SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
        Window* window = static_cast<Window*>(SDL_GetWindowData(win, "Window"));
        if (window == nullptr) {
            return 0;
        }
        window->Dispatch(*event);
    }
    return 0;
}

void App::RegisterResizer() {
    SDL_AddEventWatch(resizingEventWatcher, this);
}

void SetupBgfxPlatformData(const App& app, bgfx::PlatformData &pd, const SDL_SysWMinfo &wmi) {
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
    //pd.context = NULL;
    pd.context = app.gfx_context_;
    pd.nwh = NULL;
    pd.backBuffer = NULL;
    pd.backBufferDS = NULL;
}

} //namespace attacus