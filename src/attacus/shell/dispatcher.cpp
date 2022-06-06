#include <SDL.h>

#include "dispatcher.h"

namespace attacus {

bool Dispatcher::Dispatch(SDL_Event& event) {
    switch (event.type) {
    case SDL_QUIT:
        return false;
    case SDL_WINDOWEVENT:
        return DispatchWindowEvent(event);
    }
    return true;
}

bool Dispatcher::DispatchWindowEvent(SDL_Event& event) {
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
}
void Dispatcher::PushCallbackEvent(Delegate* delegate, void* user_data){
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = delegate;
    userevent.data2 = user_data;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
}

} //namespace attacus