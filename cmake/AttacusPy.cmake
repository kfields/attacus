include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)

function(USES_ATTPY THIS)
    USES_STANDARD(${THIS})
    target_link_libraries(${THIS} PRIVATE AttacusPy)
endfunction()
