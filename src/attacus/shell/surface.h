#pragma once

#include <string>
#include <list>
#include <map>

#include <bgfx/bgfx.h>

#include <attacus/core/event.h>

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

class SurfaceEvent : public Event {
public:
    SurfaceEvent(Surface* surface) : surface_(surface) {}
    // Data members
    Surface* surface_;
};

class Surface : public Dispatcher {
public:
    Surface(SurfaceParams params = SurfaceParams());

    template<typename T = Surface>
    static T* Produce(SurfaceParams params = SurfaceParams()) {
        T* c = new T(params);
        c->Create();
        return c;
    }

    virtual ~Surface();

    void Render() { PreRender(); Draw(); PostRender(); }
    virtual void PreRender() {}
    virtual void Draw() {}
    virtual void PostRender() {}
    void Touch() { touched_event_.Publish(SurfaceEvent(this)); }

    void SetSize(Size size);

    virtual bgfx::TextureHandle GetTexture() { return texture_; }
    virtual uintptr_t GetInternal();
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

    bgfx::TextureHandle texture_;
    EventSource<SurfaceEvent> touched_event_;
};

} //namespace attacus