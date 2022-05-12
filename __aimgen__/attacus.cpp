#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <limits>

#include <attacus/flutter/embedder.h>

#include "bindtools.h"
#include "conversions.h"

namespace py = pybind11;

void init_generated(py::module &attacus_py, Registry &registry) {
{{body}}
}