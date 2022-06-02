#pragma once

#include <attacus/shell/surface.h>

namespace attacus {

class CompositorGL;

class BackingSurfaceGL : public Surface {
public:
    BackingSurfaceGL(SurfaceParams params = SurfaceParams()) : Surface(params) {}
    void Create() override;

    template<typename T = BackingSurfaceGL>
    static T* Produce(SurfaceParams params = SurfaceParams()) {
        T* c = new T(params);
        c->Create();
        return c;
    }

    void Destroy() override;
    //Data members
    uint32_t framebuffer_id_;
    uint32_t texture_id_;
    bool used_ = false;
};

} // namespace attacus