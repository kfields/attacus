#pragma once

#include "window.h"

namespace attacus {

class GfxWindow : public Window {
public:
    GfxWindow(Window& parent, WindowParams params = WindowParams());
    virtual ~GfxWindow();
    void Create() override;
    void CreateSDLWindow() override;
    void Destroy() override;
    void PostRender() override;
    bool Dispatch(SDL_Event& event) override;
    // Accessors
    // Data members
    uint32_t resetFlags_;
    uint32_t debugFlags_;
    bool capture_ = false;
};

} //namespace attacus