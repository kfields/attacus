#include <string>
#include <iostream>

#include "SDL.h"
#include "SDL_syswm.h"

#include "app.h"
#include <attacus/flutter/flutter_view.h>

namespace attacus {

App::App(WindowParams params) : FlutterView(params),
    state_(State::kRunning)
    {}

App::~App() {
}

void App::Destroy() {
    FlutterView::Destroy();
}

void App::Create() {
    FlutterView::Create();
}

void App::Reset(ResetKind kind)
{
    //bgfx::reset(width(), height(), resetFlags_);
    //bgfx::setViewRect(viewId(), 0, 0, width(), height());
    for (auto child : children_) {
        child->Reset(ResetKind::kSoft);
    }
}

bool App::Dispatch(SDL_Event& event) {
    if (state_ == State::kShutdown)
        return false;

    if (event.window.windowID != windowId()) {
        WindowBase* child = windowMap_[event.window.windowID];
        if (child != nullptr)
            return windowMap_[event.window.windowID]->Dispatch(event);
    }

    capture_ = false;
    switch (event.type)
    {
        case SDL_USEREVENT:
        {
            Delegate& delegate = *static_cast<Delegate*>(event.user.data1);
            delegate();
            delete &delegate;
            event.user.data1 = nullptr;
            break;
        }
        case SDL_KEYDOWN:
        {
            int key = event.key.keysym.scancode;
            if (key == SDL_SCANCODE_F11) {
                capture_ = true;
                return true;
            }
        }
    }
    return FlutterView::Dispatch(event);
}

void App::PreRender() {
    FlutterView::PreRender();
}

void App::PostRender() {
    FlutterView::PostRender();
}

int App::Run() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (SDL_GL_LoadLibrary(nullptr) < 0) {
        printf("SDL_GL_LoadLibrary failed. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    RegisterResizer();

    FlutterView::Run();

    SDL_Quit();

    return 0;
}

static int resizingEventWatcher(void* data, SDL_Event* event) {
    if (event->type == SDL_WINDOWEVENT &&
        event->window.event == SDL_WINDOWEVENT_RESIZED) {
        SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
        Window* window = static_cast<Window*>(SDL_GetWindowData(win, "Window"));
        if (window == nullptr) {
            return 0;
        }
        window->Dispatch(*event);
    }
    return 0;
}

void App::RegisterResizer() {
    SDL_AddEventWatch(resizingEventWatcher, this);
}

} //namespace attacus