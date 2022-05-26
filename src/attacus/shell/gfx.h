#pragma once

namespace attacus {

class GfxView;

class Gfx {
public:
    Gfx(GfxView& view);
    void Create();
    // Accessors
    static Gfx& instance() { return *instance_; }
    GfxView& view() { return *view_; }
    //
    // Data members
    static Gfx* instance_;
    GfxView* view_ = nullptr;
};

} // namespace attacus