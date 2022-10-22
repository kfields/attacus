#pragma once

#include <list>

#include <attacus/flutter/components/flutter_component.h>

namespace attacus
{

  class Compositor : public FlutterComponent
  {
  public:
    Compositor(FlutterView &view);
    virtual FlutterCompositor *InitCompositor() { return nullptr; }
    //
    // Accessors
    // Data members
    FlutterCompositor compositor_;

  };

} // namespace attacus