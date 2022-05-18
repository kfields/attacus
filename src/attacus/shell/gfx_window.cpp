#include <string>
#include <iostream>
#include <fmt/core.h>

#include <SDL.h>
#include "SDL_syswm.h"


#include "gfx_window.h"

namespace attacus {

GfxWindow::GfxWindow(Window& parent, WindowParams params) : Window(parent, params) {}

GfxWindow::~GfxWindow() {
}

void GfxWindow::Create() {
    Window::Create();
}

void GfxWindow::Destroy() {
    Window::Destroy();
}

void GfxWindow::CreateSDLWindow() {
    // Request an OpenGL 4.5 context (should be core)
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    sdl_window_ = SDL_CreateWindow(name_.c_str(), x(), y(), width(), height(), flags_ | SDL_WINDOW_OPENGL);

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

    /*if (SDL_GL_SetSwapInterval(1) < 0) {
        std::cout << fmt::format("Couldn't enable vsync: {}\n", SDL_GetError());
    }

    SDL_GL_MakeCurrent(sdl_window_, nullptr);*/
}


} //namespace attacus