#include <iostream>
#include <fmt/core.h>

#include <glad/gl.h>
#include <SDL.h>
#include "SDL_syswm.h"

#include "window_base.h"

namespace attacus {

std::map<uint32_t, WindowBase*> WindowBase::windowMap_;

WindowBase::WindowBase(WindowParams params) :
    windowId_(0),
    name_(params.name), 
    origin_(params.origin),
    size_(params.size),
    flags_(params.flags),
    sdl_window_(nullptr)
{
}

WindowBase::~WindowBase() {
}

void WindowBase::Destroy() {
    UnmapWindow(windowId());
    SDL_DestroyWindow(sdl_window_);
    sdl_window_ = nullptr;
    Surface::Destroy();
}

void WindowBase::Create() {
    CreateSDLWindow();
}

void WindowBase::CreateSDLWindow() {
    sdl_window_ = SDL_CreateWindow(name_.c_str(), x(), y(), width(), height(), flags_);

    if (sdl_window_ == nullptr) {
        std::cout << fmt::format("Window could not be created: {}\n", SDL_GetError());
        return;
    }

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(sdl_window_, &wmi)) {
        std::cout << fmt::format("SDL_SysWMinfo could not be retrieved: {}\n", SDL_GetError());
        return;
    }
    SetWindowId(SDL_GetWindowID(sdl_window_));
    MapWindow(windowId(), this);

    SDL_SetWindowData(sdl_window_, "Window", this);
}

void WindowBase::Show() {
    SDL_ShowWindow(sdl_window_);
    Reset();
}

void WindowBase::SetPosition(Point origin) {
    origin_ = origin;
    SDL_SetWindowPosition(sdl_window_, origin.x, origin.y);
    OnPosition();
}

void WindowBase::OnPosition() {

}

void WindowBase::SetSize(Size size) {
    SDL_SetWindowSize(sdl_window_, size.width, size.height);
    OnSize();
}

void WindowBase::OnSize() {
    SDL_GetWindowSize(sdl_window_, &size_.width, &size_.height);
    Reset();
}

void WindowBase::OnResize(SDL_Event& event) {
    OnSize();
}

bool WindowBase::Dispatch(SDL_Event& event) {
    /*if (event.window.windowID != windowId()) {
        WindowBase* child = windowMap_[event.window.windowID];
        if (child != nullptr)
            return windowMap_[event.window.windowID]->Dispatch(event);
    }*/

    switch (event.type) {
        case SDL_QUIT:
            return false;
        case SDL_WINDOWEVENT:
            return DispatchWindowEvent(event);
        }
    return true;
}

bool WindowBase::DispatchWindowEvent(SDL_Event& event) {
    /*if (event.window.windowID != windowId()) {
        WindowBase* child = windowMap_[event.window.windowID];
        if(child != nullptr)
            return windowMap_[event.window.windowID]->DispatchWindowEvent(event);
    }*/

    Uint8 sdl_window_event = event.window.event;
    switch (sdl_window_event) {
        case SDL_WINDOWEVENT_RESIZED:
            OnResize(event);
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            OnSize();
            break;
        case SDL_WINDOWEVENT_MOVED:
        case SDL_WINDOWEVENT_SHOWN:
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            OnShow();
            break;
        /*case SDL_WINDOWEVENT_MOVED:
        case SDL_WINDOWEVENT_SHOWN:
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            Render();
            break;*/
        case SDL_WINDOWEVENT_CLOSE:
            return false;
    }
    return true;
}

} //namespace attacus