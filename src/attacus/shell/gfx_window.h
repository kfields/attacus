#pragma once

#include "window.h"

namespace attacus {

class GfxWindow : public Window {
public:
    GfxWindow(Window& parent, WindowParams params = WindowParams());
    virtual ~GfxWindow();
    virtual void Create() override;
    virtual void CreateSDLWindow() override;
    virtual void Destroy() override;
    // Accessors
    // Data members
};

} //namespace attacus