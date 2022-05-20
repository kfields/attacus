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
    uint32_t resetFlags_;
    uint32_t debugFlags_;
    bool debug_ = false;
    bool capture_ = false;
};

} //namespace attacus