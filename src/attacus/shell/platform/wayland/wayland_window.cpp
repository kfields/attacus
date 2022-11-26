#include "SDL.h"
#include "SDL_syswm.h"

#include <wayland-client.h>

#include "wayland_window.h"

namespace attacus {

WaylandWindow::~WaylandWindow() {
}

wl_surface* WaylandWindow::GetNativeHandle() {
    SDL_SysWMinfo wmInfo;
    SDL_GetWindowWMInfo(sdl_window_, &wmInfo, SDL_SYSWM_CURRENT_VERSION);
    wl_surface* hwnd = wmInfo.info.wl.surface;
    return hwnd;
}

} //namespace attacus