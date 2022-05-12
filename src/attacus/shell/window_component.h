#pragma once

#include "component.h"

namespace attacus {

class Window;

template<typename T = Window>
class WindowComponent : public Component {
public:
    WindowComponent(T& window) : window_(&window) {}
    T& window() { return *window_; }
    //
    T* window_;
};

} //namespace attacus
