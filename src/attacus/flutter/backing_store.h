#pragma once

#include <attacus/shell/surface.h>

namespace attacus {

class BackingStore : public Surface {
public:
    BackingStore(SurfaceParams params = SurfaceParams()) : Surface(params) {}
    void Create() override;
    void Destroy() override;
    //Data members
    uint32_t framebuffer_id;
    uint32_t texture_id;
};

} // namespace attacus