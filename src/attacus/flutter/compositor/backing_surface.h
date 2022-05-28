#pragma once

#include <attacus/shell/surface.h>
#include <vg/vg.h>

namespace attacus {

class Compositor;

class BackingSurface : public Surface {
public:
    BackingSurface(Compositor& comp, SurfaceParams params = SurfaceParams());
    void Create() override;

    template<typename T = BackingSurface>
    static T* Produce(Compositor& comp,SurfaceParams params = SurfaceParams()) {
        T* c = new T(comp, params);
        c->Create();
        return c;
    }

    void Destroy() override;
    // Accessors
    bool used() { return used_; }
    // Data members
    uint32_t framebuffer_id;
    uint32_t texture_id;
    bool used_ = false;

    vg::Context* vg_;
    vg::ImageHandle image_;
    vg::ImagePatternHandle imgPaint_;

};

} // namespace attacus