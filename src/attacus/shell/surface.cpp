#include "surface.h"

//#include <bgfx/bgfx.h>

namespace attacus {

//TODO:Recycle handles
uint16_t Surface::viewCount_ = 0;

Surface::Surface() :
    viewId_(viewCount_++), parent_(nullptr)
{
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

uint16_t Surface::AllocViewId() {
    return viewCount_++;
}

void Surface::FreeViewId(uint16_t id) {
    //TODO:Laziness
}


} //namespace attacus