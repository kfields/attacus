cmake_minimum_required(VERSION 3.14)

include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)

function(USES_GLAD THIS)
    USES_STD(${THIS})
    target_include_directories(${THIS} PRIVATE
        ${GLAD_ROOT}/include
    )
    target_precompile_headers(${THIS} PRIVATE ${GLAD_ROOT}/include/glad/gl.h)
    target_link_libraries(${THIS} PRIVATE Glad)

endfunction()

