#pragma once

#include <string>
#include <list>
#include <map>

#include <bgfx/bgfx.h>

#include "dispatcher.h"

namespace attacus {

class Surface;

struct Size {
    int width;
    int height;
    Size() : width(0), height(0) {}
    Size(int width, int height)
        : width(width), height(height) {}
    Size(const Size& s1) { width = s1.width; height = s1.height; }
};

struct SurfaceParams {
    SurfaceParams(Size _size = Size(800,600)) {
        size = _size;
    }
    Size size;
};

class Surface : public Dispatcher {
public:
    enum class ResetKind {
        kSoft,
        kHard
    };

    Surface(SurfaceParams params = SurfaceParams());

    template<typename T = Surface>
    static T* Produce(SurfaceParams params = SurfaceParams()) {
        T* c = new T(params);
        c->Create();
        return c;
    }

    virtual ~Surface();

    virtual void Destroy();

    void Render() { PreRender(); Draw(); PostRender(); }
    virtual void PreRender() {}
    virtual void Draw();
    virtual void PostRender() {}

    virtual void Reset(ResetKind kind = ResetKind::kHard) {}

    virtual void* CreateContext();
    //Accessors
    int width() { return size_.width; }
    int height() { return size_.height; }
    Size size() { return size_; }

    int16_t id() { return id_; }
    void SetId(int16_t id) { id_ = id; }
    //Data members
    Size size_;

    static int16_t surface_count_;
    int16_t id_;
    bgfx::FrameBufferHandle frameBuffer_;
    SDL_Window* sdl_window_ = nullptr;
    static void* current_context_;
};

} //namespace attacus