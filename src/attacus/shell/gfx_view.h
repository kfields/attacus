#pragma once

#include "view.h"

namespace attacus {

class GfxView : public View {
public:
    GfxView(View& parent, ViewParams params = ViewParams());
    virtual void Create() override;
    virtual void CreateGfx();
    virtual void InitGfx();

    virtual void PreRender() override;
    virtual void PostRender() override;

    // Data members
    static void* gfx_context_;

    uint32_t resetFlags_;
    uint32_t debugFlags_;
    int64_t timeOffset_;
    bool debug_ = false;
    bool capture_ = false;

};

} // namespace attacus