#pragma once

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
    ViewParams(std::string _name = "Attacus",
        Point _origin = Point(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED),
        Size _size = Size(800,600)
    ) : SurfaceParams(_size) {
        name = _name;
        origin = _origin;
    }
    std::string name;
    Point origin;
};

class View : public Surface {
public:
    View(ViewParams params = ViewParams());
    virtual ~View();
    virtual void Create() override;
    virtual void Destroy() override;
    // Accessors
    int x() { return origin_.x; }
    int y() { return origin_.y; }
    Point origin() { return origin_; }
    // Data members
    std::string name_;
    Point origin_;
    Surface* surface_ = nullptr;
    //
    void* context_ = nullptr;
    void* resource_context_ = nullptr;
    void* gfx_context_ = nullptr;
};

} //namespace attacus