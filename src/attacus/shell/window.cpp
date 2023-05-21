#include <string>
#include <iostream>

#include "SDL.h"
#include "SDL_syswm.h"

#include "window.h"
#include <attacus/flutter/flutter_view.h>

namespace attacus
{

    Window::Window(Window &parent, WindowParams params) : PlatformWindow(parent, params),
                                                          state_(State::kRunning)
    {
    }

    Window::~Window()
    {
    }

    void Window::OnShow()
    {
        for (auto child : children_)
        {
            child->OnShow();
        }
        PlatformWindow::OnShow();
    }

    /*bool Window::Dispatch(SDL_Event& event) {
        if (state_ == State::kShutdown)
            return false;

        return PlatformWindow::Dispatch(event);
    }

    bool Window::DispatchWindowEvent(SDL_Event& event) {
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
        case SDL_WINDOWEVENT_CLOSE:
            return false;
        }
        return true;
    }*/

    bool Window::Dispatch(SDL_Event &event)
    {
        if (state_ == State::kShutdown)
            return false;

        switch (event.type)
        {
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            OnResize(event);
            break;
        case SDL_EVENT_WINDOW_MOVED:
        case SDL_EVENT_WINDOW_SHOWN:
            break;
        case SDL_EVENT_WINDOW_EXPOSED:
            OnShow();
            break;
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            return false;
        }

        return PlatformWindow::Dispatch(event);
    }

    bool Window::ProcessEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (!Dispatch(event))
            {
                state_ = State::kShutdown;
                return false;
            }
        }
        return true;
    }

    void Window::Loop()
    {
        const int FPS = 60;
        const Uint64 frameDelay = 1000 / FPS;
        Uint64 frameStart;
        Uint64 frameTime;

        while (state_ == State::kRunning)
        {
            frameStart = SDL_GetTicks();

            if (ProcessEvents()) {
                Render();
                frameTime = SDL_GetTicks() - frameStart;
                if (frameDelay > frameTime)
                {
                    Uint32 delay = (Uint32)(frameDelay - frameTime);
                    SDL_Delay(delay);
                }
            }
        }
    }

    int Window::Run()
    {
        PlatformWindow::Run();
        Loop();
        Shutdown();
        Destroy();
        return 0;
    }

} // namespace attacus