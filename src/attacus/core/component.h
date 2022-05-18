#pragma once

#include <map>
#include <functional>

namespace attacus {

class Delegate {
public:
    Delegate() {}
    Delegate(std::function<void(void)> callback) : callback_(callback) {}
    void operator()() {
        callback_();
    }
    // Data members
    std::function<void(void)> callback_;
};

class Component {
public:
    virtual void Create();
    virtual void Destroy();
    virtual void CleanUp() {}

    virtual int Run();
    virtual void Startup();
    virtual void Shutdown();

    template<typename T = Component>
    static T* Produce() {
        T* c = new T();
        c->Create();
        return c;
    }
    //
    // Data members
    std::map<const char*, Component*> components_;
};

class ComponentKit {
public:
    static const char* Intern(const char* name);
    // Accessors
    static ComponentKit& instance();
    // Data members
    std::map<const char*, const char*> internalized_;
private:
    static ComponentKit* instance_;
};

} //namespace attacus
