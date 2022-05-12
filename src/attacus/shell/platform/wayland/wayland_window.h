#pragma once

#include <functional>
#include <memory>
#include <string>

typedef unsigned long XWindow;

#include <attacus/shell/window_base.h>

struct wl_surface;

namespace attacus {

class WaylandWindow : public WindowBase {
 public:

    WaylandWindow(WindowParams params = WindowParams()) : WindowBase(params) {}
    virtual ~WaylandWindow();

    wl_surface* GetNativeHandle();
    virtual void* GetHandle() override { return (void*)GetNativeHandle(); }
};

} //namespace attacus