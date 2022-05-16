#include "surface.h"

namespace attacus {

//TODO:Recycle handles
int16_t Surface::surface_count_ = 0;

Surface::Surface(SurfaceParams params, Surface* parent) :
    size_(params.size), parent_(parent),
    id_(surface_count_++), frameBuffer_(BGFX_INVALID_HANDLE)
{
    if (parent == nullptr) {
        return;
    }
    parent->AddChild(*this);
    sdl_window_ = parent->sdl_window_;
}

Surface::~Surface() {
}

void Surface::Destroy() {
    for (auto child : children_) {
        child->parent_ = nullptr;
        child->Destroy();
    }
    if (parent_ != nullptr) {
        parent_->RemoveChild(*this);
    }

    delete this;
}

void Surface::Draw() {
    for (std::list<Surface*>::iterator it = children_.begin(); it != children_.end(); ++it){
        Surface* child = *it;
        child->Draw();
    }
}

} //namespace attacus