cmake_minimum_required(VERSION 3.14)

include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Glad.cmake)

function(USES_SDL THIS)
    USES_STD(${THIS})
    USES_GLAD(${THIS})
    target_include_directories(${THIS} PRIVATE
        ${SDL_ROOT}/include
    )
    #target_link_libraries(${THIS} PRIVATE SDL2main SDL2-static)
    target_link_libraries(${THIS} PRIVATE SDL2main SDL2)
endfunction()

