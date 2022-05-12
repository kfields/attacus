cmake_minimum_required(VERSION 3.4)

project(AttacusPy VERSION 0.1 LANGUAGES CXX)

#if(SKBUILD)
#  execute_process(
#    COMMAND "${PYTHON_EXECUTABLE}" -c
#            "import pybind11; print(pybind11.get_cmake_dir())"
#    OUTPUT_VARIABLE _tmp_dir
#    OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
#  list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")
#endif()

# Now we can find pybind11
#find_package(pybind11 CONFIG REQUIRED)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/Pybind11.cmake)
include(${ATT_ROOT}/cmake/Attacus.cmake)

set(ATTPY_HEADERS
    bindings/bindtools.h
    bindings/conversions.h
)

set(ATTPY_SOURCES
    #bindings/generated.cpp
    bindings/main.cpp
)

pybind11_add_module(${THIS} MODULE
    bindings/init.cpp
)

set_target_properties(${THIS} PROPERTIES OUTPUT_NAME "attacus_py")

USES_PYBIND11(${THIS})
USES_ATT(${THIS})

#target_compile_definitions(${THIS} PRIVATE ATT_DLL)

#

target_sources(${THIS} PRIVATE ${ATTPY_SOURCES})
# add include files
target_sources(${THIS} PRIVATE ${ATTPY_HEADERS}) # for intellisense
target_include_directories(${THIS}
	PUBLIC
		$<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
