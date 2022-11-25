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
    target_precompile_headers(${THIS} PRIVATE ${SDL_ROOT}/include/SDL.h)
    target_link_libraries(${THIS} PRIVATE SDL3main SDL3-static)
    #target_link_libraries(${THIS} PRIVATE SDL3main SDL3)
endfunction()

