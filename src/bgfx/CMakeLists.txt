cmake_minimum_required(VERSION 3.4)

project(bgfx LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(../../cmake/Standard.cmake)

set(BX_SRC
  ${BX_ROOT}/src/amalgamated.cpp
)

set(OPTION_ASTC_TESTS OFF CACHE BOOL "Build all the unit tests" FORCE)

add_subdirectory(${BIMG_ROOT}/3rdparty/astc-codec astc-codec)

set(BIMG_SRC
  ${BIMG_ROOT}/src/image.cpp
  ${BIMG_ROOT}/src/image_gnf.cpp
  ${BIMG_ROOT}/src/image_decode.cpp
  ${BIMG_ROOT}/3rdparty/tinyexr/deps/miniz/miniz.c
)

set(BGFX_HEADERS
    ${BGFX_ROOT}/include/bgfx/bgfx.h
    bgfx_p.h
)

set(_ ${BGFX_ROOT}/src)

set(BGFX_SRC
    #${BGFX_ROOT}/src/amalgamated.cpp
    #${_}/bgfx.cpp
    bgfx.cpp
    ${_}/vertexlayout.cpp
    ${_}/shader.cpp
    ${_}/topology.cpp
    ${_}/debug_renderdoc.cpp
    ${_}/renderer_noop.cpp
    ${_}/renderer_agc.cpp
    ${_}/renderer_d3d9.cpp
    ${_}/renderer_d3d11.cpp
    ${_}/renderer_d3d12.cpp
    ${_}/renderer_gnm.cpp
    ${_}/renderer_nvn.cpp
    ${_}/renderer_gl.cpp
    ${_}/renderer_webgpu.cpp

    ${_}/glcontext_egl.cpp
    ${_}/glcontext_glx.cpp
    ${_}/glcontext_html5.cpp
    ${_}/glcontext_wgl.cpp

    ${_}/shader_dxbc.cpp
    ${_}/shader_dx9bc.cpp
    ${_}/shader_spirv.cpp
)
if(${ATT_WM_WAYLAND})
set(BGFX_SRC ${BGFX_SRC}
    renderer_vk.cpp
)
else()
set(BGFX_SRC ${BGFX_SRC}
    ${_}/renderer_vk.cpp
)
endif()

if(WIN32)
set(BGFX_SRC ${BGFX_SRC}
    ${_}/nvapi.cpp
    ${_}/dxgi.cpp
)
endif()

add_library(${THIS} STATIC
  ${BX_SRC}
  ${BX_ROOT}/include
  ${BIMG_SRC}
  ${BIMG_ROOT}/include
  ${BGFX_SRC}
  ${BGFX_ROOT}/include
)
USES_STD(${THIS})

if(BX_COMPATIBILITY)
    target_compile_definitions(${THIS} PRIVATE -D__STDC_FORMAT_MACROS=1)
endif()

install(TARGETS ${THIS} EXPORT BgfxTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# add include files
target_sources(${THIS} PRIVATE ${BGFX_HEADERS}) # for intellisense
target_include_directories(${THIS}
	PUBLIC
		$<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${BGFX_ROOT}>
	PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${BX_COMPATIBILITY}
        ${BX_ROOT}/include
        ${BX_ROOT}/3rdparty
        ${BIMG_ROOT}/include
        ${BIMG_ROOT}/3rdparty
        ${BIMG_ROOT}/3rdparty/tinyexr/deps/miniz
        ${BGFX_ROOT}/include
        ${BGFX_ROOT}/src
        ${BGFX_ROOT}/3rdparty
        ${BGFX_ROOT}/3rdparty/khronos
        ${BGFX_ROOT}/3rdparty/dxsdk/include
)

target_link_libraries(${THIS} PRIVATE astc-codec)

add_subdirectory(utils)