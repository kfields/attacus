cmake_minimum_required(VERSION 3.4)

project(bimg LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(../../cmake/Standard.cmake)

set(BIMG_SRC
  ${BIMG_ROOT}/src/image.cpp
  ${BIMG_ROOT}/src/image_gnf.cpp
  ${BIMG_ROOT}/src/image_decode.cpp
  ${BIMG_ROOT}/3rdparty/tinyexr/deps/miniz/miniz.c
)

set(BIMG_HEADERS
    ${BIMG_ROOT}/include/bimg/bimg.h
)

add_library(${THIS} STATIC
  ${BIMG_SRC}
)
USES_STD(${THIS})

if(BX_COMPATIBILITY)
    target_compile_definitions(${THIS} PRIVATE -D__STDC_FORMAT_MACROS=1)
endif()

install(TARGETS ${THIS} EXPORT BImgTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# add include files
target_sources(${THIS} PRIVATE ${BGFX_HEADERS}) # for intellisense
target_include_directories(${THIS}
	PUBLIC
		$<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${BIMG_ROOT}>
	PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${BX_COMPATIBILITY}
        ${BX_ROOT}/include
        ${BX_ROOT}/3rdparty
        ${BIMG_ROOT}/include
        ${BIMG_ROOT}/3rdparty
        ${BIMG_ROOT}/3rdparty/astc-codec/include
        ${BIMG_ROOT}/3rdparty/tinyexr/deps/miniz
)

target_link_libraries(${THIS} PRIVATE astc-codec)

add_subdirectory(astc-codec)
