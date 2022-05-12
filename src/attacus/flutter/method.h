#pragma once

#include <functional>

#include "engine_method_result.h"
#include "method_call.h"
#include "method_codec.h"
#include "method_result.h"
#include "json.h"

namespace attacus {

class EncodableValue;

// A handler for receiving a method call from the Flutter engine.
//
// Implementations must asynchronously call exactly one of the methods on
// |result| to indicate the result of the method call.
template <typename T>
using MethodCallHandler =
    std::function<void(const MethodCall<T> &call,
                        std::unique_ptr<MethodResult<T>> result)>;

template <typename T>
class MethodChannel;

template <typename T = EncodableValue>
class Method
{
public:
    Method(MethodChannel<T> &channel, const std::string &name, const MethodCallHandler<T> call_handler);

    ~Method() = default;

    virtual void HandleCall(const MethodCall<T>& method_call, std::unique_ptr<EngineMethodResult<T>> result) {
        call_handler_(method_call, std::move(result));
    }

    //Data members
    MethodChannel<T>* channel_;
    std::string name_;
    const MethodCallHandler<T> call_handler_;
};

typedef Method<EncodableValue> StandardMethod;
typedef Method<JsonValue> JsonMethod;

} //namespace attacus