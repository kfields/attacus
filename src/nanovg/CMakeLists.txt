cmake_minimum_required(VERSION 3.4)

project(NanoVG LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(../../cmake/Bgfx.cmake)

set(NVG_HEADERS
    nanovg.h
    nanovg_gl.h
)

set(NVG_SRC
    nanovg.cpp
    nanovg_bgfx.cpp
)

add_library(${THIS} STATIC
  ${NVG_SRC} ${NVG_HEADERS}
)
USES_BGFX(${THIS})
USES_BGFX_UTILS(${THIS})