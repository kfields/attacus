#pragma once

#include <string>
#include <list>
#include <map>

#include <bgfx/bgfx.h>

#include "component.h"

namespace attacus {

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

class Surface : public Component {
public:
    enum class ResetKind {
        kSoft,
        kHard
    };

    Surface(SurfaceParams params = SurfaceParams(), Surface* parent = nullptr);

    template<typename T = Surface>
    static T& Produce(SurfaceParams params = SurfaceParams(), Surface* parent = nullptr) {
        T& c = *new T(parent);
        c.Create();
        return c;
    }

    virtual ~Surface();

    virtual void Destroy();

    void AddChild(Surface& child) {
        child.parent_ = this;
        children_.push_back(&child);
    }

    void RemoveChild(Surface& child) {
        child.parent_ = nullptr;
        children_.remove(&child);
    }

    void Render() { PreRender(); Draw(); PostRender(); }
    virtual void PreRender() {}
    virtual void Draw() {}
    virtual void PostRender() {}

    virtual void Reset(ResetKind kind = ResetKind::kHard) {}

    uint16_t AllocViewId();
    void FreeViewId(uint16_t id);

    //Accessors
    int width() { return size_.width; }
    int height() { return size_.height; }
    Size size() { return size_; }

    uint16_t viewId() { return viewId_; }
    void SetViewId(uint16_t id) { viewId_ = id; }
    //Data members
    Size size_;

    static uint16_t viewCount_;
    uint16_t viewId_;
    Surface* parent_;
    std::list<Surface*> children_;
    bgfx::FrameBufferHandle frameBuffer_;
};

} //namespace attacus