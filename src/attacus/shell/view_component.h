#pragma once

#include "dispatcher.h"

namespace attacus {

class View;

template<typename T = View>
class ViewComponent : public Dispatcher {
public:
    ViewComponent(T& view) : view_(&view) {}
    T& view() { return *view_; }
    //
    T* view_;
};

} //namespace attacus
