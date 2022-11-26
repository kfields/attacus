#include <windows.h>
#define SDL_ENABLE_SYSWM_WINDOWS
#include <SDL_syswm.h>

#include "win32_window.h"

namespace attacus
{

Win32Window::~Win32Window()
{
}

void Win32Window::Create()
{
    WindowBase::Create();
}

RECT Win32Window::GetClientArea()
{
    RECT frame;
    GetClientRect(GetNativeHandle(), &frame);
    return frame;
}

HWND Win32Window::GetNativeHandle()
{
    SDL_SysWMinfo wmInfo;
    SDL_GetWindowWMInfo(sdl_window_, &wmInfo, SDL_SYSWM_CURRENT_VERSION);
    HWND hwnd = wmInfo.info.win.window;
    return hwnd;
}

void Win32Window::AttachTo(WindowBase &parent)
{
    WindowBase::AttachTo(parent);
    //NativeAttachTo((HWND)parent.GetHandle());
    SDL_SetWindowModalFor(sdl_window_, parent.sdl_window_);
}
int Win32Window::NativeAttachTo(HWND hParent)
{
    SetPosition(Point(0, 0));

    HWND hWnd = GetNativeHandle();
    SetParent(hWnd, hParent);
    const LONG nNewStyle = (GetWindowLong(hWnd, GWL_STYLE) & ~WS_POPUP & ~WS_OVERLAPPEDWINDOW) | WS_CHILDWINDOW | WS_VISIBLE;
    SetWindowLong(hWnd, GWL_STYLE, nNewStyle);
    const ULONG_PTR cNewStyle = GetClassLongPtr(hWnd, GCL_STYLE) | CS_DBLCLKS;
    SetClassLongPtr(hWnd, GCL_STYLE, cNewStyle);
    ShowWindow(hWnd, SW_NORMAL);

    return 0;
}

} // namespace attacus