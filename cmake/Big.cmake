include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Shell.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Gui.cmake)

function(USES_BIG THIS)
    USES_GUI(${THIS})
    USES_SHELL(${THIS})
    target_link_libraries(${THIS} BigSaddle)

endfunction()
