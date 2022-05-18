#include <iostream>

#include <attacus/flutter/flutter_view.h>

#include "view_kit.h"

namespace attacus
{

ViewKit* ViewKit::instance_ = new ViewKit();

ViewKit::ViewKit() {}

ViewKit::~ViewKit() = default;

int16_t ViewKit::RegisterView(View& View)
{
    view_count++;
    View.id_ = view_count;
    views_[view_count] = &View;
    //FlutterEngineResult result = FlutterEngineRegisterExternalView(engine_, view_id);
    return view_count;
}

bool ViewKit::UnregisterView(int16_t view_id)
{
    //FlutterEngineResult result = FlutterEngineUnregisterExternalView(engine_, view_id);
    return true;
}

} // namespace attacus