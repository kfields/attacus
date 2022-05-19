#pragma once

#include <string>
#include <list>
#include <map>

#include <bgfx/bgfx.h>

#include "surface.h"

namespace attacus {

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    Point(const Point& p1) { x = p1.x; y = p1.y; }
};

struct ViewParams : SurfaceParams {
    ViewParams(
        Size _size = Size(800,600),
        Point _origin = Point(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED),
        std::string _name = "Attacus"
    ) : SurfaceParams(_size) {
        name = _name;
        origin = _origin;
    }
    std::string name;
    Point origin;
};

class View : public Surface {
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

    bool Dispatch(SDL_Event& event) override;

    void Draw() override;

    virtual void Reset(ResetKind kind = ResetKind::kHard) {}

    virtual void* CreateContext();
    //
    void SetPosition(Point origin);
    virtual void OnPosition();
    //Accessors

    int16_t viewId() { return view_id_; }
    void SetViewId(int16_t id) { view_id_ = id; }
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
    std::list<View*> children_;

    static int16_t view_count_;
    int16_t view_id_;
    bgfx::FrameBufferHandle frameBuffer_;
    SDL_Window* sdl_window_ = nullptr;
    static void* current_context_;
};

} //namespace attacus