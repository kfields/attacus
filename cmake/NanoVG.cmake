include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/bgfx/Utils.cmake)

function(USES_NVG THIS)
    USES_BGFX_UTILS(${THIS})
    target_include_directories(${THIS} PRIVATE
        ${NVG_ROOT}
    )
    target_link_libraries(${THIS} PRIVATE NanoVG)

endfunction()
