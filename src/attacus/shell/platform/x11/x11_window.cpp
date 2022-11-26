#include <cstring>

#include "SDL.h"
#define SDL_ENABLE_SYSWM_X11
#include "SDL_syswm.h"

#include "x11_window.h"

namespace attacus {

X11Window::~X11Window() {
}

XWindow X11Window::GetNativeHandle() {
    SDL_SysWMinfo wmInfo;
    SDL_SysWMinfo wmInfo;
    SDL_GetWindowWMInfo(sdl_window_, &wmInfo, SDL_SYSWM_CURRENT_VERSION);
    XWindow hwnd = wmInfo.info.x11.window;
    return hwnd;
}

} //namespace attacus