#pragma once

#include <glad/gl.h>

namespace attacus {

class GfxView;

class Gfx {
public:
    Gfx(GfxView& view);
    void Create();
    void Reset();
    // Accessors
    static Gfx& instance() { return *instance_; }
    GfxView& view() { return *view_; }
    //
    // Data members
    static Gfx* instance_;
    GfxView* view_ = nullptr;
    bool debug_ = false;
    uint32_t resetFlags_;
    uint32_t debugFlags_;
    // OpenGL
    GLADloadfunc gl_proc_resolver_ = nullptr;
};

} // namespace attacus