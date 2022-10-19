#pragma once

#include "window.h"

namespace attacus
{

  class GfxWindow : public Window
  {
  public:
    GfxWindow(Window &parent, WindowParams params = WindowParams());
    virtual ~GfxWindow();
    void Create() override;
    void CreateSDLWindow() override;
    void Destroy() override;
    void PreRender() override;
    void PostRender() override;
    bool Dispatch(SDL_Event &event) override;
    // Accessors
    // Data members
    bool capture_ = false;
  };

} // namespace attacus