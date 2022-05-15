#include <iostream>

#include <attacus/flutter/flutter_view.h>

#include "surface_manager.h"

namespace attacus
{

SurfaceManager* SurfaceManager::instance_ = new SurfaceManager();

SurfaceManager::SurfaceManager() {}

SurfaceManager::~SurfaceManager() = default;

int16_t SurfaceManager::RegisterSurface(Surface& Surface)
{
    surface_count++;
    Surface.id_ = surface_count;
    surfaces_[surface_count] = &Surface;
    //FlutterEngineResult result = FlutterEngineRegisterExternalSurface(engine_, surface_id);
    return surface_count;
}

bool SurfaceManager::UnregisterSurface(int16_t surface_id)
{
    //FlutterEngineResult result = FlutterEngineUnregisterExternalSurface(engine_, surface_id);
    return true;
}

} // namespace attacus