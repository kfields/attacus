#include <string>
#include <iostream>
#include <fmt/core.h>

#include <glad/gl.h>
#include <SDL.h>
#include "SDL_syswm.h"

#include "view.h"

namespace attacus {

//TODO:Recycle handles
int16_t View::surface_count_ = 0;
void* View::current_context_ = nullptr;

View::View(View& parent, ViewParams params) : Dispatcher(),
    origin_(params.origin),
    name_(params.name),
    size_(params.size),
    parent_(&parent),
    id_(surface_count_++), frameBuffer_(BGFX_INVALID_HANDLE) {
    if (parent_ == nullptr) {
        return;
    }
    sdl_window_ = parent.sdl_window_;
    parent.AddChild(*this);
}

View::~View() {
}

void View::Create() {
    Dispatcher::Create();
}

void* View::CreateContext() {
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
        std::cout << fmt::format("Can't create opengl context: {}\n", SDL_GetError());
        return nullptr;
    }

    return context;
}

int View::Run() {
    Dispatcher::Run();
    for (std::list<View*>::iterator it = children_.begin(); it != children_.end(); ++it){
        View* child = *it;
        child->Run();
    }
    return 0;
}

void View::Destroy() {
    for (auto child : children_) {
        child->parent_ = nullptr;
        child->Destroy();
    }
    if (parent_ != nullptr) {
        parent_->RemoveChild(*this);
    }
    Dispatcher::Destroy();
}

bool View::Dispatch(SDL_Event& event) {
    for (std::list<View*>::iterator it = children_.begin(); it != children_.end(); ++it){
        View* child = *it;
        child->Dispatch(event);
    }
    return Dispatcher::Dispatch(event);
}

void View::Draw() {
    for (std::list<View*>::iterator it = children_.begin(); it != children_.end(); ++it){
        View* child = *it;
        child->Render();
    }
}

void View::SetPosition(Point origin) {
    origin_ = origin;
    OnPosition();
}

void View::OnPosition() {

}

void View::SetSize(Size size) {
    size_ = size;
    OnSize();
}

} //namespace attacus