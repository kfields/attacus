include_guard()

# Standard includes
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)

set(ATT_STATIC_PIC ON)
#
# Directories
#
set(ATT_ROOT ${CMAKE_SOURCE_DIR})

set(GLM_ROOT ${ATT_ROOT}/3rdparty/glm)
set(PYBIND11_ROOT ${ATT_ROOT}/3rdparty/pybind11)
set(RXCPP_ROOT ${ATT_ROOT}/3rdparty/rxcpp)
set(GLAD_ROOT ${ATT_ROOT}/src/glad)
set(FLUTTER_ROOT ${ATT_ROOT}/src/flutter)


set(BX_ROOT ${ATT_ROOT}/3rdparty/bx)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
set(BX_COMPATIBILITY ${BX_ROOT}/include/compat/msvc)
endif()

set(BIMG_ROOT ${ATT_ROOT}/3rdparty/bimg)
set(BGFX_ROOT ${ATT_ROOT}/3rdparty/bgfx)

set(NVG_ROOT ${ATT_ROOT}/src/nanovg)
set(VGR_ROOT ${ATT_ROOT}/3rdparty/vg-renderer)

set(EXAMPLES_ROOT ${ATT_ROOT}/examples)

set(GOOGLE_TEST_ROOT ${ATT_ROOT}/3rdparty/googletest)

#
# Platform
#
set(ATT_PLATFORM ${CMAKE_SYSTEM_NAME})

set(ATT_PLATFORM_WINDOWS OFF)
set(ATT_PLATFORM_LINUX OFF)

if(${ATT_PLATFORM} STREQUAL "Windows")
    set(ATT_PLATFORM_WINDOWS ON)
elseif(${ATT_PLATFORM} STREQUAL "Linux")
    set(ATT_PLATFORM_LINUX ON)
endif()

#
# Renderer
#

#option(ATT_RENDERER_GL "Use GL" OFF)
#cmake_dependent_option(ATT_RENDERER_VULKAN "Use Vulkan" ON
#                       "ATT_PLATFORM_LINUX; NOT ATT_RENDERER_GL" OFF)

set(ATT_RENDERER_GL ON)
set(ATT_RENDERER_VULKAN OFF)

option(ATT_WM_WAYLAND "Use Wayland" OFF)
cmake_dependent_option(ATT_WM_X11 "Use X11" ON "ATT_PLATFORM_LINUX; NOT ATT_WM_WAYLAND" OFF)


set(ATT_COMPILE_DEFS 
    NOMINMAX=1
)

if(${ATT_WM_X11})
    set(ATT_COMPILE_DEFS ${ATT_COMPILE_DEFS}
        SDL_VIDEO_DRIVER_X11=1
    )
elseif(${ATT_WM_WAYLAND})
    set(ATT_COMPILE_DEFS ${ATT_COMPILE_DEFS}
        SDL_VIDEO_DRIVER_WAYLAND=1
    )
endif()

if(${ATT_RENDERER_GL})
    set(ATT_COMPILE_DEFS ${ATT_COMPILE_DEFS}
        BGFX_CONFIG_RENDERER_OPENGL=1
        BGFX_CONFIG_RENDERER_OPENGLES=0
        BGFX_CONFIG_RENDERER_VULKAN=0
    )
elseif(${ATT_RENDERER_VULKAN})
    set(ATT_COMPILE_DEFS ${ATT_COMPILE_DEFS}
        BGFX_CONFIG_RENDERER_OPENGL=0
        BGFX_CONFIG_RENDERER_OPENGLES=0
        BGFX_CONFIG_RENDERER_VULKAN=1
    )
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(BX_COMPATIBILITY ${BX_ROOT}/include/compat/msvc)
endif()

# Comment out items to minimize builds for testing
set(HAS_APP 1)
set(HAS_SHELL 1)
set(HAS_FLUTTER 1)
set(HAS_BGFX 1)
set(HAS_PYBIND11 1)
set(HAS_SDL 1)
set(HAS_GLAD 1)
set(HAS_PY 1)
set(HAS_EXAMPLES 1)
