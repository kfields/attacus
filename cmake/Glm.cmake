include_guard()

function(USES_GLM THIS)
  target_include_directories(${THIS} PRIVATE
    ${GLM_ROOT}
  )
  target_link_libraries(${THIS} PRIVATE glm)

endfunction()
