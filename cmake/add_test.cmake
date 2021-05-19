macro(nn_add_test programName)
    add_executable(${programName} ${programName}.cpp)
    target_link_libraries(${programName} fmt::fmt GTest::gtest GTest::gtest_main Eigen3::Eigen3 range-v3::range-v3)
    target_compile_features(${programName} PUBLIC cxx_std_20)

    include(GoogleTest)
    gtest_discover_tests(${programName})

    if(NOT TARGET check)
        add_custom_target(
          check COMMAND ${CMAKE_CTEST_COMMAND}
          DEPENDS ${programName}
        )
    else()
        add_dependencies(check ${programName})
    endif()
endmacro()
