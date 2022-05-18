#include "frame.h"

namespace attacus {

Frame::Frame(View& parent, ViewParams params) : View(parent, params)
{
}

void Frame::Create() {
}

void Frame::OnSize() {
    for (std::list<View*>::iterator it = children_.begin(); it != children_.end(); ++it){
        View* child = *it;
        child->SetSize(size());
    }
}

} //namespace attacus