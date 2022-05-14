#include "surface.h"

namespace attacus {

//TODO:Recycle handles
uint16_t Surface::viewCount_ = 0;

Surface::Surface(SurfaceParams params, Surface* parent) :
    size_(params.size), parent_(parent),
    viewId_(viewCount_++), frameBuffer_(BGFX_INVALID_HANDLE)
{
    if (parent != nullptr) {
        parent->AddChild(*this);
    }
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
uint16_t Surface::AllocViewId() {
    return viewCount_++;
}

void Surface::FreeViewId(uint16_t id) {
    //TODO:Laziness
}


} //namespace attacus