#pragma once

#include <attacus/shell/surface.h>

namespace attacus {

class CompositorGfx;

class BackingSurfaceGfx : public Surface {
public:
    BackingSurfaceGfx(CompositorGfx& comp, SurfaceParams params = SurfaceParams());
    void Create() override;

    template<typename T = BackingSurfaceGfx>
    static T* Produce(CompositorGfx& comp,SurfaceParams params = SurfaceParams()) {
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

};

} // namespace attacus