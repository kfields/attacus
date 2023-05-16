#include <iostream>
#include <format>

#include <SDL.h>
#include "SDL_syswm.h"

#include "window_base.h"

namespace attacus {

std::map<uint32_t, WindowBase*> WindowBase::windowMap_;

WindowBase::WindowBase(WindowBase& parent, WindowParams params) : Frame(parent, params),
    windowId_(0),
    flags_(params.flags)
{
}

WindowBase::~WindowBase() {
}

void WindowBase::CleanUp() {
    Frame::CleanUp();
    UnmapWindow(windowId());
    SDL_DestroyWindow(sdl_window_);
    sdl_window_ = nullptr;
}

void WindowBase::Create() {
    CreateSDLWindow();
}

void WindowBase::CreateSDLWindow() {
    sdl_window_ = SDL_CreateWindowWithPosition(name_.c_str(), x(), y(), width(), height(), flags_);

    if (sdl_window_ == nullptr) {
        std::cout << std::format("Window could not be created: {}\n", SDL_GetError());
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
    Frame::OnSize();
    Reset();
}

void WindowBase::OnResize(SDL_Event& event) {
    OnSize();
}

void WindowBase::ResizeChildren() {
    int cur_width, cur_height;
    SDL_GetWindowSizeInPixels(sdl_window_, &cur_width, &cur_height);

    for (auto child : children_){
        child->SetSize(Size(cur_width, cur_height));
    }
}

} //namespace attacus