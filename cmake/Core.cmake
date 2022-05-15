include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)

function(USES_CORE THIS)
    USES_STD(${THIS})
    target_link_libraries(${THIS} PRIVATE AttCore)
endfunction()