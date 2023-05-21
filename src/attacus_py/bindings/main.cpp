#include <limits>
//#include <filesystem>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "bindtools.h"

#include <attacus/app.h>
#include <attacus/flutter/flutter_view.h>
#include <attacus/flutter/flutter_messenger.h>
#include <attacus/flutter/standard_method_channel.h>

namespace py = pybind11;

using namespace attacus;

template<class T>
struct always_false : std::false_type {};

EncodableValue encode(py::object obj) {
    PyObject* object = obj.ptr();
    EncodableValue value;
    if (Py_IsNone(object)) {
        value = nullptr;
    }
    else if (PyLong_Check(object)) {
        int64_t val = PyLong_AsLong(object);
        value = val;
    }
    else if (PyUnicode_Check(object)) {
        auto val = PyUnicode_AsUTF8(object);
        value = val;
    }
    return value;
}

class PyApp : public App {
public:
    /* Inherit the constructors */
    using App::App;
    PyApp() : App() {}
    /* Trampoline (need one for each virtual function) */
    void Loop() override {
        PYMETHOD_OVERRIDE(
            void,       // Return type
            App,        // Parent class
            loop,    // Python function name
            Loop     // C++ function name
        );
    }
    void Startup() override {
        PYMETHOD_OVERRIDE(
            void,       // Return type
            App,        // Parent class
            startup,    // Python function name
            Startup     // C++ function name
        );
    }
    void Shutdown() override {
        PYMETHOD_OVERRIDE(
            void,
            App,
            shutdown,
            Shutdown
        );
    }
};

class PyFlutterView : public FlutterView {
public:
    /* Inherit the constructors */
    using FlutterView::FlutterView;
    PyFlutterView(View& parent, ViewParams params = ViewParams()) : FlutterView(parent) {}
    /* Trampoline (need one for each virtual function) */
    void Startup() override {
        PYMETHOD_OVERRIDE(
            void,           // Return type
            FlutterView,    // Parent class
            startup,        // Python function name
            Startup         // C++ function name
        );
    }
    void Shutdown() override {
        PYMETHOD_OVERRIDE(
            void,
            FlutterView,
            shutdown,
            Shutdown
        );
    }
};

void init_main(py::module &attacus_py, Registry &registry) {

    PYCLASS_O_BEGIN(attacus_py, App, PyApp)
        .def(py::init<>([](){
            return App::Produce<PyApp>();
        }))
        .def("run", [](App& self) {
            self.Run();
        })
        .def("process_events", &App::ProcessEvents)
        .def("startup", &App::Startup)
        .def("shutdown", &App::Shutdown)
    PYCLASS_END(attacus_py, App)

    //PYCLASS_BEGIN(attacus_py, FlutterView)
    PYCLASS_O_BEGIN(attacus_py, FlutterView, PyFlutterView)
        .def(py::init<>([](App& parent){
            return FlutterView::Produce<PyFlutterView>(parent);
        }))
        .def("startup", &FlutterView::Startup)
        .def("shutdown", &FlutterView::Shutdown)
        .def_readonly("messenger", &FlutterView::messenger_)
    PYCLASS_END(attacus_py, FlutterView)

    PYCLASS_BEGIN(attacus_py, FlutterMessenger)
    PYCLASS_END(attacus_py, FlutterMessenger)


    auto _StandardMethod = py::class_<StandardMethod, std::unique_ptr<StandardMethod, py::nodelete>>(attacus_py, "StandardMethod")
        .def(py::init<>([](StandardMethodChannel& channel, const std::string& name, py::object cb){
            std::cout << "StandardMethod: " << name << std::endl;
            std::cout << cb << std::endl;
            return new StandardMethod(
                channel,
                name,
                [cb](const StandardMethodCall &call, std::unique_ptr<StandardMethodResult> result){
                    std::cout << "StandardMethodCall" << std::endl;
                    std::cout << cb << std::endl;
                    auto py_call = py::cast(&call);
                    std::cout << "py_call: " << py_call << std::endl;
                    auto py_result = py::cast(result.get());
                    std::cout << "py_result: " << py_result << std::endl;
                    cb(py_call, py_result);
                }
            );
        }))
    PYCLASS_END(attacus_py, StandardMethod)

    PYCLASS_BEGIN(attacus_py, StandardMethodCall)
        .def("method_name", &StandardMethodCall::method_name)
        .def("arguments", &StandardMethodCall::arguments)
    PYCLASS_END(attacus_py, StandardMethodCall)

    auto _StandardMethodResult = py::class_<StandardMethodResult, std::unique_ptr<StandardMethodResult, py::nodelete>>(attacus_py, "StandardMethodResult")
        .def("success", [](StandardMethodResult& self, py::object obj) {
            PyObject* object = obj.ptr();
            if (Py_IsNone(object)) {
                return self.Success();
            }
            if (PyLong_Check(object)) {
                int64_t val = PyLong_AsLong(object);
                return self.Success(val);
            }
        }
        , py::arg("value") = nullptr
        )
    PYCLASS_END(attacus_py, StandardMethodResult)

    //PYCLASS_BEGIN(attacus_py, EncodableValue)
    auto _EncodableValue = py::class_<EncodableValue, std::unique_ptr<EncodableValue, py::nodelete>>(attacus_py, "EncodableValue")
        .def("decode", [](EncodableValue& self) {
                auto variant = self;
                return std::visit([](auto&& arg) -> py::object {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, int>) {
                        return py::int_(arg);
                    } else if constexpr (std::is_same_v<T, double>) {
                        return py::float_(arg);
                    } else if constexpr (std::is_same_v<T, std::string>) {
                        return py::str(arg);
                    } else {
                        //static_assert(always_false<T>::value, "non-exhaustive visitor!");
                        return py::none();
                    }
                }, variant);
            })
    PYCLASS_END(attacus_py, EncodableValue)

    PYCLASS_BEGIN(attacus_py, StandardMethodChannel)
        .def(py::init<>([](FlutterMessenger& messenger, const std::string& name){
            std::cout << "StandardMethodChannel: " << name << std::endl;
            return new StandardMethodChannel(
                messenger,
                name
            );
        }))
        .def("invoke_method", [](StandardMethodChannel& self, const std::string &method, py::object obj) {
            EncodableValue value = encode(obj);
            std::unique_ptr<EncodableValue> args = std::make_unique<EncodableValue>(value);
            return self.InvokeMethod(method, std::move(args));
        }
            , py::arg("name")
            , py::arg("arguments")
            //, py::arg("result") = nullptr
            , py::return_value_policy::automatic_reference
        )




    PYCLASS_END(attacus_py, StandardMethodChannel)
   
}
