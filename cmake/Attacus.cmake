include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Shell.cmake)

function(USES_ATT THIS)
    USES_SHELL(${THIS})
    target_link_libraries(${THIS} PRIVATE Attacus)
endfunction()
