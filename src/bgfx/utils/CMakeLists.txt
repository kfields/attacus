cmake_minimum_required(VERSION 3.4)

project(BgfxUtils LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/Bgfx.cmake)

set(UTIL_HEADERS
)

set(UTIL_SRC
    stb.cpp
    utils.cpp
    entry.cpp
    ${BGFX_ROOT}/3rdparty/meshoptimizer/src/indexcodec.cpp
    ${BGFX_ROOT}/3rdparty/meshoptimizer/src/vertexcodec.cpp
)

add_library(${THIS} ${UTIL_SRC} ${UTIL_HEADERS})
USES_BGFX(${THIS})

target_include_directories(${THIS}
PRIVATE
)
