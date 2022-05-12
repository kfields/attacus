#pragma once

#include "window.h"

namespace attacus {

class View : public Window {
public:
    View(WindowParams params = WindowParams());
    virtual ~View();
    virtual void Create() override;
    virtual void CreateSDLWindow() override;
    virtual void Destroy() override;
    // Accessors
    // Data members
};

} //namespace attacus