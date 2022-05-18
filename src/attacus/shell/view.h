#pragma once

#include <string>
#include <list>
#include <map>

#include <bgfx/bgfx.h>

#include "dispatcher.h"

namespace attacus {

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    Point(const Point& p1) { x = p1.x; y = p1.y; }
};

struct Size {
    int width;
    int height;
    Size() : width(0), height(0) {}
    Size(int width, int height)
        : width(width), height(height) {}
    Size(const Size& s1) { width = s1.width; height = s1.height; }
};

struct ViewParams {
    ViewParams(
        Size _size = Size(800,600),
        Point _origin = Point(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED),
        std::string _name = "Attacus"
    ) {
        name = _name;
        size = _size;
        origin = _origin;
    }
    std::string name;
    Point origin;
    Size size;
};

class View : public Dispatcher {
public:
    enum class ResetKind {
        kSoft,
        kHard
    };

    View(View& parent, ViewParams params = ViewParams());

    template<typename T = View>
    static T* Produce(View& parent, ViewParams params = ViewParams()) {
        T* c = new T(parent, params);
        c->Create();
        return c;
    }
    
    virtual ~View();
    void Create() override;
    void Destroy() override;
    int Run() override;

    virtual bool Dispatch(SDL_Event& event) override;

    void Render() { PreRender(); Draw(); PostRender(); }
    virtual void PreRender() {}
    virtual void Draw();
    virtual void PostRender() {}

    virtual void Reset(ResetKind kind = ResetKind::kHard) {}

    virtual void* CreateContext();
    //
    void SetPosition(Point origin);
    virtual void OnPosition();
    void SetSize(Size size);
    //Accessors
    int width() { return size_.width; }
    int height() { return size_.height; }
    Size size() { return size_; }

    int16_t id() { return id_; }
    void SetId(int16_t id) { id_ = id; }
    View& parent() { return *parent_; }

    void AddChild(View& child) {
        child.parent_ = this;
        children_.push_back(&child);
    }

    void RemoveChild(View& child) {
        child.parent_ = nullptr;
        children_.remove(&child);
    }

    // Accessors
    int x() { return origin_.x; }
    int y() { return origin_.y; }
    Point origin() { return origin_; }
    // Data members
    std::string name_;
    Point origin_;
    View* parent_;
    Size size_;

    static int16_t surface_count_;
    int16_t id_;
    bgfx::FrameBufferHandle frameBuffer_;
    SDL_Window* sdl_window_ = nullptr;
    //
    static void* current_context_;

    //
    std::list<View*> children_;
};

} //namespace attacus