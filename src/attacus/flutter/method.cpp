#include "method.h"

namespace attacus {

template <typename T>
Method<T>::Method(MethodChannel<T> &channel,
                const std::string &name,
                const MethodCallHandler<T> call_handler)
    : channel_(channel), name_(name), call_handler_(call_handler) {
        channel.AddChannel(*this);
    }

}