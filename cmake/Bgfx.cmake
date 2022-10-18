include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/bgfx/Utils.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/bgfx/Shader.cmake)

function(USES_BGFX THIS)
    USES_STD(${THIS})

    target_include_directories(${THIS} PRIVATE
        ${BX_COMPATIBILITY}
        ${BX_ROOT}/include
        ${BX_ROOT}/3rdparty

        ${BIMG_ROOT}/include
        ${BIMG_ROOT}/3rdparty

        ${BGFX_ROOT}/include
        ${BGFX_ROOT}/src
        ${BGFX_ROOT}/3rdparty
        ${BGFX_ROOT}/3rdparty/khronos
        ${BGFX_ROOT}/examples/common
    )
    target_link_libraries(${THIS} PRIVATE bgfx)
endfunction()