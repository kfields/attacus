#pragma once

#include "view.h"

namespace attacus {

class Frame : public View {
public:
    Frame(View& parent, ViewParams params = ViewParams());
    void Create() override;
    void OnSize() override;
    // Data members
};

} // namespace attacus