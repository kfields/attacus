#pragma once

namespace attacus {

class GfxView;

class Gfx {
public:
    Gfx(GfxView& view);
    void Create();
    void SetupBgfxPlatformData(bgfx::PlatformData &pd, const SDL_SysWMinfo &wmi);
    // Accessors
    static Gfx& instance() { return *instance_; }
    GfxView& view() { return *view_; }
    //
    // Data members
    static Gfx* instance_;
    GfxView* view_ = nullptr;
};

} // namespace attacus