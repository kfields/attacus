#include <iostream>

#include <attacus/flutter/flutter_view.h>

#include "surface_kit.h"

namespace attacus
{

SurfaceKit* SurfaceKit::instance_ = new SurfaceKit();

SurfaceKit::SurfaceKit() {}

SurfaceKit::~SurfaceKit() = default;

int16_t SurfaceKit::RegisterSurface(Surface& Surface)
{
    view_count++;
    Surface.id_ = view_count;
    views_[view_count] = &Surface;
    //FlutterEngineResult result = FlutterEngineRegisterExternalSurface(engine_, view_id);
    return view_count;
}

bool SurfaceKit::UnregisterSurface(int16_t view_id)
{
    //FlutterEngineResult result = FlutterEngineUnregisterExternalSurface(engine_, view_id);
    return true;
}

} // namespace attacus