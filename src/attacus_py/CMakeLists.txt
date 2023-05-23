cmake_minimum_required(VERSION 3.4)

project(AttacusPy VERSION 0.1 LANGUAGES CXX)

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
