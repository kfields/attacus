#pragma once

#include "window.h"

namespace attacus {

class GfxWindow : public Window {
public:
    GfxWindow(WindowParams params = WindowParams());
    virtual ~GfxWindow();
    virtual void Create() override;
    virtual void CreateSDLWindow() override;
    virtual void CreateGfx() {}
    virtual void Destroy() override;
    // Accessors
    // Data members
    void* context_ = nullptr;
    void* resource_context_ = nullptr;
    void* gfx_context_ = nullptr;
};

} //namespace attacus