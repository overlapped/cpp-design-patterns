function(add_pattern std name)
    set(dir "${CMAKE_CURRENT_SOURCE_DIR}/${std}/${name}")
    if(NOT EXISTS "${dir}")
        message(WARNING "Pattern directory not found: ${dir}")
        return()
    endif()

    # Example
    if(BUILD_EXAMPLES AND EXISTS "${dir}/example.cpp")
        add_executable(example_${std}_${name} "${dir}/example.cpp")
        target_link_libraries(example_${std}_${name} PRIVATE patterns)
        target_include_directories(example_${std}_${name} PRIVATE "${dir}")
    endif()

    # Test
    if(BUILD_TESTS AND EXISTS "${dir}/test.cpp")
        add_executable(test_${std}_${name} "${dir}/test.cpp")
        target_link_libraries(test_${std}_${name} PRIVATE patterns GTest::gtest_main)
        target_include_directories(test_${std}_${name} PRIVATE "${dir}")
        include(GoogleTest)
        gtest_discover_tests(test_${std}_${name})
    endif()
endfunction()