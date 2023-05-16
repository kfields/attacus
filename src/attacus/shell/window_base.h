#pragma once

#include <string>
#include <list>
#include <map>
#include <thread>

#include "frame.h"

namespace attacus {

struct WindowParams : ViewParams {
    WindowParams(
        Size _size = Size(800,600),
        Point _origin = Point(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED),
        std::string _name = "Attacus",
        uint32_t _flags = SDL_WINDOW_RESIZABLE
    ) : ViewParams(_size, _origin, _name) {
        flags = _flags;
    }
    uint32_t flags;
};

class WindowBase : public Frame
{
public:
    WindowBase(WindowBase& parent, WindowParams params = WindowParams());
    virtual ~WindowBase();

    virtual void Create();
    virtual void CreateSDLWindow();
    void CleanUp() override;

    virtual void Show();
    virtual void Hide() {}

    /*
    virtual bool Dispatch(SDL_Event& event);
    virtual bool DispatchWindowEvent(SDL_Event& event) override;
    */

    void OnSize() override;
    //
    virtual void* GetHandle() { return nullptr; }
    virtual void AttachTo(WindowBase& parent) { parent_ = &parent; }
    //
    void SetPosition(Point origin);
    void OnPosition() override;
    void SetSize(Size size);
    void OnResize(SDL_Event& event) override;
    void ResizeChildren() override;
    //
    static void MapWindow(uint32_t key, WindowBase* window) { windowMap_[key] = window; }
    static void UnmapWindow(uint32_t key) { windowMap_.erase(key); }
    //Accessors
    uint32_t windowId() { return windowId_; }
    void SetWindowId(uint32_t id) { windowId_ = id; }
    //Data members
    WindowBase* parent_ = nullptr;
    uint32_t windowId_;
    static std::map<uint32_t, WindowBase*> windowMap_;
    uint32_t flags_;
};

} //namespace attacus