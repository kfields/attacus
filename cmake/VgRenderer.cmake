include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/bgfx/Utils.cmake)

function(USES_VGR THIS)
    USES_BGFX_UTILS(${THIS})
    target_include_directories(${THIS} PRIVATE
        ${VGR_ROOT}/include
    )
    target_link_libraries(${THIS} PRIVATE VgRenderer)
endfunction()
