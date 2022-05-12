#include <iostream>
#include <map>
#include <functional>
#include <pybind11/pybind11.h>

namespace py = pybind11;

typedef std::function<void(py::detail::generic_type& _class)> RegistryCallback;

class Registry {
    public:
    std::map<std::string, RegistryCallback> callbacks;
    void addCallback(const std::string name, RegistryCallback callback) {
        this->callbacks[name] = callback;
    }
    void on(py::module& _module, const std::string name, py::detail::generic_type& _class) {
        //std::cout << "on register:  " << name << "\n";
        if ( this->callbacks.find(name) == this->callbacks.end() ) { return; }
        this->callbacks[name](_class);
    }
};

//#define PYCLASS_BEGIN(_module, _class, _name) py::class_<_class> _##_name = py::class_<_class>(_module, #_name)
#define PYCLASS_BEGIN(_module, _class) py::class_<_class> _##_class = py::class_<_class>(_module, #_class)

#define PYCLASS_O_BEGIN(_module, _class, _oclass) py::class_<_class, _oclass> _##_class = py::class_<_class, _oclass>(_module, #_class)

//#define PYCLASS_END(_module, _class, _name) ; registry.on(_module, #_name, _##_name);
#define PYCLASS_END(_module, _class) ; registry.on(_module, #_class, _##_class);

#define PYEXTEND_BEGIN(_class, name) \
    registry.addCallback(#name, [](py::detail::generic_type& _type) { \
        py::class_<_class> &name = (py::class_<_class>&)_type;

#define PYEXTEND_END });

//
#define PYMETHOD_OVERRIDE(ret_type, cname, pyfn, fn, ...)                                               \
    PYBIND11_OVERRIDE_NAME(PYBIND11_TYPE(ret_type), PYBIND11_TYPE(cname), #pyfn, fn, __VA_ARGS__)
