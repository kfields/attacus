#include <glad/gl.h>
#include "SDL.h"
#include "SDL_syswm.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "gfx_view.h"
#include "gfx.h"

namespace attacus
{

  Gfx *Gfx::instance_ = nullptr;

  Gfx::Gfx(GfxView &view) : view_(&view),
                            resetFlags_(BGFX_RESET_VSYNC),
                            debugFlags_(BGFX_DEBUG_TEXT)

  {
    instance_ = this;
  }

  void Gfx::Create()
  {
    auto result = bgfx::renderFrame(); // single threaded mode

    bgfx::PlatformData pd{};
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(view().sdl_window_, &wmInfo);

    SetupBgfxPlatformData(pd, wmInfo);

    bgfx::Init bgfx_init;
    bgfx_init.debug = true;
    // bgfx_init.type = bgfx::RendererType::Count; // auto choose renderer
    bgfx_init.type = bgfx::RendererType::OpenGL;
    bgfx_init.resolution.width = view().width();
    bgfx_init.resolution.height = view().height();
    bgfx_init.resolution.reset = resetFlags_;
    bgfx_init.platformData = pd;
    bgfx::init(bgfx_init);
  }

  void Gfx::SetupBgfxPlatformData(bgfx::PlatformData &pd, const SDL_SysWMinfo &wmi)
  {
    switch (wmi.subsystem)
    {
    case SDL_SYSWM_UNKNOWN:
      abort();

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
    }

    pd.context = NULL;
    if (view().view_id_ != 0)
    {
      pd.nwh = NULL;
      pd.context = view().gfx_context_;
    }
    pd.backBuffer = NULL;
    pd.backBufferDS = NULL;
  }

  void Gfx::Reset() {
    bgfx::reset(view().width(), view().height(), resetFlags_);
  }
} // namespace attacus