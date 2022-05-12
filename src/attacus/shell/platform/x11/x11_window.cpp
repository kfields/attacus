#include <cstring>

#include "SDL.h"
#include "SDL_syswm.h"

#include "x11_window.h"

namespace attacus {

X11Window::~X11Window() {
}

XWindow X11Window::GetNativeHandle() {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window_, &wmInfo);
    XWindow hwnd = wmInfo.info.x11.window;
    return hwnd;
}

} //namespace attacus