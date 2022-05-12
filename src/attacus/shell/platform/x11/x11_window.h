#pragma once

#include <functional>
#include <memory>
#include <string>

//#include <X11/Xlib.h>
typedef unsigned long XWindow;

#include <attacus/shell/window_base.h>

namespace attacus {

class X11Window : public WindowBase {
 public:

    X11Window(WindowParams params = WindowParams()) : WindowBase(params) {}
    virtual ~X11Window();

    XWindow GetNativeHandle();
    virtual void* GetHandle() override { return (void*)GetNativeHandle(); }
};

} //namespace attacus