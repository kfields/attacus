#include <attacus/flutter/flutter_view.h>

#include "flutter_component.h"

namespace attacus {

FlutterComponent::FlutterComponent(FlutterView& view) : ViewComponent(view) {}

void FlutterComponent::Create() {
    engine_ = flutter().engine_;
}

} // namespace attacus