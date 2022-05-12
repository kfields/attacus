#pragma once

#include <functional>
#include <memory>
#include <string>

#include <windows.h>

#include <attacus/shell/window_base.h>

namespace attacus {

class Win32Window : public WindowBase {
 public:

    Win32Window(WindowParams params = WindowParams()) : WindowBase(params) {}
    virtual ~Win32Window();

    virtual void Create() override;
    virtual void AttachTo(WindowBase& parent) override;
    int NativeAttachTo(HWND hParent);
    HWND GetNativeHandle();
    virtual void* GetHandle() override { return (void*)GetNativeHandle(); }
    RECT GetClientArea();

};

} //namespace attacus {