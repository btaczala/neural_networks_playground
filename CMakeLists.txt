project(neural_experiments C CXX)
cmake_minimum_required(VERSION 3.20)

enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()
include(conan_config.cmake)
include(cmake/add_test.cmake)

find_package(fmt REQUIRED)
find_package(GTest REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(range-v3 REQUIRED)

nn_add_test(network_test)
nn_add_test(mnist_reader_test)
nn_add_test(image_recognition_test)

add_executable(picture_printer picture_printer.cpp)
target_compile_features(picture_printer PUBLIC cxx_std_20)
target_link_libraries(picture_printer Eigen3::Eigen3 range-v3::range-v3 fmt::fmt)
target_compile_definitions(picture_printer PRIVATE MNIST_DIR="${CMAKE_SOURCE_DIR}")
