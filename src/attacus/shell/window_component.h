#pragma once

#include "dispatcher.h"

namespace attacus {

class Window;

template<typename T = Window>
class WindowComponent : public Dispatcher {
public:
    WindowComponent(T& window) : window_(&window) {}
    T& window() { return *window_; }
    //
    T* window_;
};

} //namespace attacus
