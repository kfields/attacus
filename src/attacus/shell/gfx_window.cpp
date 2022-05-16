#include <string>
#include <iostream>
#include <fmt/core.h>

#include <glad/gl.h>
#include <SDL.h>
#include "SDL_syswm.h"

#include "SDL.h"
#include "SDL_syswm.h"

#include "gfx_window.h"

namespace attacus {

GfxWindow::GfxWindow(WindowParams params) : Window(params) {}

GfxWindow::~GfxWindow() {
}

void GfxWindow::Create() {
    Window::Create();
    CreateGfx();
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

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

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

    gfx_context_ = SDL_GL_CreateContext(sdl_window_);
    if (gfx_context_ == NULL) {
        std::cout << fmt::format("Can't create opengl context for bgfx: {}\n", SDL_GetError());
        return;
    }

    resource_context_ = SDL_GL_CreateContext(sdl_window_);
    if (resource_context_ == NULL) {
        std::cout << fmt::format("Can't create opengl context for resource window: {}\n", SDL_GetError());
        return;
    }

    context_ = SDL_GL_CreateContext(sdl_window_);
    if (context_ == NULL) {
        std::cout << fmt::format("Can't create opengl context: {}\n", SDL_GetError());
        return;
    }

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    std::cout << fmt::format("OpenGL {}.{} loaded\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    SDL_SetWindowData(sdl_window_, "Window", this);
    SDL_SetWindowData(sdl_window_, "GL", context_);
    SDL_SetWindowData(sdl_window_, "GL2", resource_context_);

    if (SDL_GL_SetSwapInterval(1) < 0) {
        std::cout << fmt::format("Couldn't enable vsync: {}\n", SDL_GetError());
    }
}


} //namespace attacus