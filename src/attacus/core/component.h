#pragma once

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

    template<typename T = Component>
    static T& Produce() {
        T& c = *new T();
        c.Create();
        return c;
    }
};

} //namespace attacus
