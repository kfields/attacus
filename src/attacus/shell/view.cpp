#include <string>
#include <iostream>
#include <fmt/core.h>

#include <glad/gl.h>
#include <SDL.h>
#include "SDL_syswm.h"

#include "view.h"

namespace attacus {

//TODO:Recycle handles
int16_t View::view_count_ = 1; //Reserve 0 for fullscreen/bgfx

View::View(View& parent, ViewParams params) : Surface(params),
    origin_(params.origin),
    name_(params.name),
    parent_(&parent),
    view_id_(view_count_++) {
    if (parent_ == nullptr) {
        return;
    }
    sdl_window_ = parent.sdl_window_;
    sdl_renderer_ = parent.sdl_renderer_;
    parent.AddChild(*this);
}

View::~View() {
}

void View::Create() {
    Surface::Create();
}

int View::Run() {
    Surface::Run();
    for (std::vector<View*>::iterator it = children_.begin(); it != children_.end(); ++it){
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
    Surface::Destroy();
}

bool View::Dispatch(SDL_Event& event) {
    for (std::vector<View*>::iterator it = children_.begin(); it != children_.end(); ++it){
        View* child = *it;
        child->Dispatch(event);
    }
    return Surface::Dispatch(event);
}

void View::Draw() {
    for (std::vector<View*>::iterator it = children_.begin(); it != children_.end(); ++it){
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

bgfx::TextureHandle View::GetTexture() {
    return bgfx::getTexture(frameBuffer_); 
}

} //namespace attacus