cmake_minimum_required(VERSION 3.4)

project(VgRenderer LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(../../cmake/Bgfx.cmake)

set(_ ${VGR_ROOT}/include/vg)

set(VGR_HEADERS
    ${_}/path.h
    ${_}/stroker.h
    ${_}/vg.h
)

set(_ ${VGR_ROOT}/src)
set(VGR_SRC
    ${_}/path.cpp
    ${_}/stroker.cpp
    ${_}/vg.cpp
    ${_}/vg_util.cpp
    ${_}/libs/fontstash.cpp
    ${_}/libtess2/tess.c
    ${_}/libtess2/bucketalloc.c
    ${_}/libtess2/mesh.c
    ${_}/libtess2/sweep.c
    ${_}/libtess2/geom.c
    ${_}/libtess2/dict.c
    ${_}/libtess2/priorityq.c
)

add_library(${THIS} STATIC
    ${VGR_SRC} ${VGR_HEADERS}
)

target_include_directories(${THIS} PRIVATE
    ${VGR_ROOT}/include
)

USES_BGFX(${THIS})
USES_BGFX_UTILS(${THIS})