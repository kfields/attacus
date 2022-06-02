cmake_minimum_required(VERSION 3.4)

include(./cmake/Config.cmake)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /O2 /bigobj")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#
# fmt
#
add_subdirectory(3rdparty/fmt)

#
# GLM
#
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static library" FORCE)

add_subdirectory(3rdparty/glm)

#
# pybind11
#
set(-DCMAKE_CXX_STANDARD=20 -std=c++20)
set(PYBIND11_PYTHON_VERSION 3.10)

if(${HAS_PYBIND11})
add_subdirectory(3rdparty/pybind11)
endif() #HAS_PYBIND11
#

#
# SDL - Minimal build for now
#
set(HAVE_LIBC ON)
set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library" FORCE)
#set(SDL_STATIC_PIC ON CACHE BOOL "Static version of the library should be built with Position Independent Code" FORCE)

#set(SDL_RENDER OFF CACHE BOOL "Use render subsystem" FORCE)
set(SDL_AUDIO OFF CACHE BOOL "Use audio subsystem" FORCE)
set(SDL_HAPTIC OFF CACHE BOOL "Use haptic subsystem" FORCE)
set(HIDAPI OFF CACHE BOOL "Use HIDAPI for low level joystick drivers" FORCE)

set(VIDEO_X11 ${ATT_WM_X11} CACHE BOOL "Enable X11 support" FORCE)
set(VIDEO_WAYLAND ${ATT_WM_WAYLAND} CACHE BOOL "Enable Wayland support" FORCE)

if(${HAS_SDL})
add_subdirectory(3rdparty/sdl)
endif() #HAS_SDL

#
# Glad
#
if(${ATT_RENDERER_GL})
    #add_subdirectory(${GLAD_ROOT}/cmake glad)
    #glad_add_library(glad_gl_core_45 STATIC API gl:core=4.5)
    if(${HAS_GLAD})
    add_subdirectory(src/glad)
    endif() #HAS_GLAD
endif()

#
# bgfx
#
if(${HAS_BGFX})
add_subdirectory(src/bgfx)
endif() #HAS_BGFX

#
# NanoVG
#
add_subdirectory(src/nanovg)

#
# VgRenderer
#
add_subdirectory(src/vg-renderer)

#
#Attacus
#
add_subdirectory(src/attacus)

#
#AttacusPy
#
if(${HAS_PY})
add_subdirectory(src/attacus_py)
endif() #HAS_PY

#
#Examples
#
if(${HAS_EXAMPLES})
add_subdirectory(examples)
endif() #HAS_EXAMPLES

#
# googletest
#
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(3rdparty/googletest)

#
#Tests
#
add_subdirectory(tests)