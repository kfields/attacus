#include <attacus/flutter/flutter_window.h>

#include "flutter_component.h"

namespace attacus {

FlutterComponent::FlutterComponent(FlutterWindow& view) : WindowComponent(view) {}

void FlutterComponent::Create() {
    engine_ = flutter().engine_;
}

} // namespace attacus