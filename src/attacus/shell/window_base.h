#pragma once

#include <string>
#include <list>
#include <map>
#include <thread>

#include <SDL.h>

#include "surface.h"

namespace attacus {

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    Point(const Point& p1) { x = p1.x; y = p1.y; }
};

struct WindowParams : SurfaceParams {
    WindowParams(std::string _name = "Attacus",
        Point _origin = Point(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED),
        Size _size = Size(800,600),
        uint32_t _flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    ) : SurfaceParams(_size) {
        name = _name;
        origin = _origin;
        flags = _flags;
    }
    std::string name;
    Point origin;
    uint32_t flags;
};

class WindowBase : public Surface
{
public:
    WindowBase(WindowParams params = WindowParams());
    virtual ~WindowBase();

    virtual void Create();
    virtual void CreateSDLWindow();
    virtual void Destroy() override;

    void CreateAndShow() {
        Create();
        Show();
    }
    virtual void Show();
    virtual void Hide() {}

    virtual bool Dispatch(SDL_Event& event);
    virtual bool DispatchWindowEvent(SDL_Event& event) override;

    virtual void OnSize();
    //
    virtual void* GetHandle() { return nullptr; }
    virtual void AttachTo(WindowBase& parent) { parent_ = &parent; }
    //
    void SetPosition(Point origin);
    virtual void OnPosition();
    void SetSize(Size size);
    virtual void OnResize(SDL_Event& event) override;
    //
    static void MapWindow(uint32_t key, WindowBase* window) { windowMap_[key] = window; }
    static void UnmapWindow(uint32_t key) { windowMap_.erase(key); }
    //Accessors
    uint32_t windowId() { return windowId_; }
    void SetWindowId(uint32_t id) { windowId_ = id; }
    int x() { return origin_.x; }
    int y() { return origin_.y; }
    Point origin() { return origin_; }
    //Data members
    WindowBase* parent_ = nullptr;
    uint32_t windowId_;
    static std::map<uint32_t, WindowBase*> windowMap_;
    std::string name_;
    Point origin_;
    uint32_t flags_;
    SDL_Window* sdl_window_;
    SDL_Window* sdl_resource_window_ = nullptr;
};

} //namespace attacus