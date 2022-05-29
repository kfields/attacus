#pragma once

#include "view.h"

namespace attacus {

class Gfx;

class GfxView : public View {
public:
    GfxView(View& parent, ViewParams params = ViewParams());
    virtual void Create() override;
    virtual void* CreateContext();
    virtual void CreateGfx();
    virtual void InitGfx();
    void CreateFramebuffer() override;
    void Reset(ResetKind kind = ResetKind::kHard) override;

    virtual void PreRender() override;
    virtual void PostRender() override;

    // Accessors
    Gfx& gfx() { return *gfx_; }
    // Data members
    Gfx* gfx_ = nullptr;
    static void* current_context_;
    static void* gfx_context_;

    uint32_t resetFlags_;
    uint32_t debugFlags_;
    int64_t time_offset_;
    bool debug_ = false;
    bool capture_ = false;

};

} // namespace attacus