#include <string>
#include <iostream>
#include <fmt/core.h>

#include "surface.h"

#include "view.h"

namespace attacus {

//TODO:Recycle handles
int16_t Surface::surface_count_ = 0;
void* Surface::current_context_ = nullptr;

Surface::Surface(Surface* parent, SurfaceParams params) :
    size_(params.size),
    parent_(parent),
    id_(surface_count_++), frameBuffer_(BGFX_INVALID_HANDLE)
{
    if (parent == nullptr) {
        return;
    }
    sdl_window_ = parent->sdl_window_;
}

void* Surface::CreateContext() {
    void* context = nullptr;
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    if (current_context_) {
        SDL_GL_MakeCurrent(sdl_window_, current_context_);
        context = SDL_GL_CreateContext(sdl_window_);    
    } else {
        context = SDL_GL_CreateContext(sdl_window_);    
    }
    current_context_ = context;
    if (context == NULL) {
        std::cout << fmt::format("Can't create opengl context for bgfx: {}\n", SDL_GetError());
        return nullptr;
    }
    return context;
}

Surface::~Surface() {
}

void Surface::Destroy() {
    delete this;
}

void Surface::Draw() {
}

} //namespace attacus