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

class PyApp : public App {
public:
    /* Inherit the constructors */
    using App::App;
    PyApp() : App() {}
    /* Trampoline (need one for each virtual function) */
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
    //PyFlutterView() : FlutterView() {}
    //PyFlutterView(const PyFlutterView&);
    //PyFlutterView(View& parent, ViewParams params = ViewParams()) : FlutterView(parent, params) {}
    PyFlutterView(View& parent, ViewParams params = ViewParams()) : FlutterView(parent) {}
    /* Trampoline (need one for each virtual function) */
    void Startup() override {
        PYMETHOD_OVERRIDE(
            void,       // Return type
            FlutterView,        // Parent class
            startup,    // Python function name
            Startup     // C++ function name
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

/*class PyStandardMethodChannel : public StandardMethodChannel {
public:
    PyStandardMethodChannel(FlutterMessenger& messenger, const std::string& name, py::object cb)
        : StandardMethodChannel(
            messenger,
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
        )
        , cb_(cb) {}
    py::object cb_;
};*/

void init_main(py::module &attacus_py, Registry &registry) {

    PYCLASS_O_BEGIN(attacus_py, App, PyApp)
        .def(py::init<>([](){
            return App::Produce<PyApp>();
        }))
        .def("run", [](App& self) {
            self.Run();
        })
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

    PYCLASS_BEGIN(attacus_py, StandardMethodChannel)
        .def(py::init<>([](FlutterMessenger& messenger, const std::string& name){
            std::cout << "StandardMethodChannel: " << name << std::endl;
            return new StandardMethodChannel(
                messenger,
                name
            );
        }))
    PYCLASS_END(attacus_py, StandardMethodChannel)

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
            //PyTypeObject* type = Py_TYPE(object);
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
    PYCLASS_END(attacus_py, EncodableValue)
    
}
