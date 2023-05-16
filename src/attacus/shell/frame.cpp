#include "frame.h"

namespace attacus {

Frame::Frame(View& parent, ViewParams params) : View(parent, params)
{
}

void Frame::Create() {
}

void Frame::ResizeChildren() {
    for (auto child : children_){
        child->SetSize(size());
    }
}

} //namespace attacus