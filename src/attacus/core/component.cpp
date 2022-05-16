#include "component.h"

namespace attacus {

void Component::Create() {}


ComponentKit* ComponentKit::instance_ = nullptr;

ComponentKit& ComponentKit::instance() {
    if (instance_ == nullptr) {
        instance_ = new ComponentKit();
    }
    return *instance_;
}
const char* ComponentKit::Intern(const char* name) {
    if (instance().internalized_.count(name) > 0) {
        return instance().internalized_[name];
    } else {
        instance().internalized_[name] = name;
        return name;
    }
}


} //namespace attacus
