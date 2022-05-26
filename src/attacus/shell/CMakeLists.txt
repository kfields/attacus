cmake_minimum_required(VERSION 3.4)

project(AttShell LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/Sdl.cmake)
include(${ATT_ROOT}/cmake/Flutter.cmake)
include(${ATT_ROOT}/cmake/Bgfx.cmake)


set(SHELL_HEADERS
    dispatcher.h
    window_component.h
    surface.h
    view_kit.h
    view.h
    frame.h
    window_base.h
    platform_window.h
    window.h
    gfx_window.h
    gfx_view.h
    gfx.h
)

set(SHELL_SRC
    dispatcher.cpp
    surface.cpp
    view_kit.cpp
    view.cpp
    frame.cpp
    window_base.cpp
    window.cpp
    gfx_window.cpp
    gfx_view.cpp
    gfx.cpp
)

add_library(${THIS} STATIC ${SHELL_SRC} ${SHELL_HEADERS})
set_target_properties(${THIS} PROPERTIES POSITION_INDEPENDENT_CODE ${ATT_STATIC_PIC})

USES_SDL(${THIS})
USES_FLUTTER(${THIS})
USES_BGFX(${THIS})

target_compile_definitions(${THIS} PRIVATE SHELL_HEADER=${SHELL_HEADER} SHELL_CLASS=${SHELL_CLASS})
target_link_libraries(${THIS} PRIVATE AttShell${ATT_PLATFORM})

if(${ATT_PLATFORM_WINDOWS})
    add_subdirectory(platform/windows)
elseif(${ATT_PLATFORM_LINUX})
    if(${ATT_WM_X11})
        add_subdirectory(platform/x11)
    elseif(${ATT_WM_WAYLAND})
        add_subdirectory(platform/wayland)
    endif()
endif()