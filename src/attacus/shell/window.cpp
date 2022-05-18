#include <string>
#include <iostream>

#include "SDL.h"
#include "SDL_syswm.h"

#include "window.h"
#include <attacus/flutter/flutter_view.h>

namespace attacus {

Window::Window(Window& parent, WindowParams params) : PlatformWindow(parent, params),
    state_(State::kRunning)
    {}

Window::~Window() {
}

void Window::Create() {
    PlatformWindow::Create();
}

void Window::Destroy() {
    PlatformWindow::Destroy();
}

void Window::Reset(ResetKind kind)
{
    //bgfx::reset(width(), height(), resetFlags_);
    //bgfx::setViewRect(viewId(), 0, 0, width(), height());
    for (auto child : children_) {
        child->Reset(ResetKind::kSoft);
    }
}

bool Window::Dispatch(SDL_Event& event) {
    if (state_ == State::kShutdown)
        return false;

    //flutter().Dispatch(event);
    capture_ = false;
    switch (event.type)
    {
        case SDL_KEYDOWN:
        {
            int key = event.key.keysym.scancode;
            if (key == SDL_SCANCODE_F11) {
                capture_ = true;
                return true;
            }
        }
    }
    return PlatformWindow::Dispatch(event);
}

int Window::Run() {
    PlatformWindow::Run();
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    while (state_ == State::kRunning) {
        frameStart = SDL_GetTicks();

        SDL_Event event;
        
        while (SDL_PollEvent(&event)) {
        //while (SDL_WaitEvent(&event) != 0) {
            if (!Dispatch(event)) {
                state_ = State::kShutdown;
            }
        }

        Render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    Shutdown();
    Destroy();

    return 0;
}

} //namespace attacus