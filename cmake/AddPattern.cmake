function(add_pattern std name)
  # name может содержать слэш, например solid/srp
  set(dir "${CMAKE_CURRENT_SOURCE_DIR}/${std}/${name}")
  string(REPLACE "/" "_" flat_name "${name}")   # solid/srp -> solid_srp

  if(NOT EXISTS "${dir}")
    message(WARNING "Pattern directory not found: ${dir}")
    return()
  endif()

  set(example_target "example_${std}_${flat_name}")
  set(test_target    "test_${std}_${flat_name}")

  # Example
  if(BUILD_EXAMPLES AND EXISTS "${dir}/example.cpp")
    add_executable(${example_target} "${dir}/example.cpp")
    target_link_libraries(${example_target} PRIVATE patterns Threads::Threads)
    target_include_directories(${example_target} PRIVATE "${dir}")
  endif()

  # Test
  if(BUILD_TESTS AND EXISTS "${dir}/test.cpp")
    add_executable(${test_target} "${dir}/test.cpp")
    target_link_libraries(${test_target}
                          PRIVATE patterns GTest::gtest_main Threads::Threads)
    target_include_directories(${test_target} PRIVATE "${dir}")
    include(GoogleTest)
    gtest_discover_tests(${test_target})
  endif()

  if(COMMAND enable_sanitizers)
    if(TARGET ${example_target})
      enable_sanitizers(${example_target})
    endif()
    if(TARGET ${test_target})
      enable_sanitizers(${test_target})
    endif()
    if(TARGET ${std}_${flat_name}_impl)
      enable_sanitizers(${std}_${flat_name}_impl)
    endif()
  endif()
endfunction()
