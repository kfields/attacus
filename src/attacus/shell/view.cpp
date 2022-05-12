#include <string>
#include <iostream>
#include <fmt/core.h>

#include <glad/gl.h>
#include <SDL.h>
#include "SDL_syswm.h"

#include "SDL.h"
#include "SDL_syswm.h"

#include "view.h"

namespace attacus {

View::View(WindowParams params) : Window(params) {}

View::~View() {
}

void View::Create() {
    Window::Create();
}

void View::Destroy() {
    Window::Destroy();
}

void View::CreateSDLWindow() {
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

    auto resource_context = SDL_GL_CreateContext(sdl_window_);
    if (resource_context == NULL) {
        std::cout << fmt::format("Can't create opengl context for resource window: {}\n", SDL_GetError());
        return;
    }

    auto context = SDL_GL_CreateContext(sdl_window_);
    if (context == NULL) {
        std::cout << fmt::format("Can't create opengl context: {}\n", SDL_GetError());
        return;
    }

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    std::cout << fmt::format("OpenGL {}.{} loaded\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    SDL_SetWindowData(sdl_window_, "Window", this);
    SDL_SetWindowData(sdl_window_, "GL", context);
    SDL_SetWindowData(sdl_window_, "GL2", resource_context);

    if (SDL_GL_SetSwapInterval(1) < 0) {
        std::cout << fmt::format("Couldn't enable vsync: {}\n", SDL_GetError());
    }
}


} //namespace attacus