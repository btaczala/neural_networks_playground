
include(conan.cmake)
conan_cmake_configure(
  REQUIRES
  fmt/6.1.2
  gtest/1.10.0
  eigen/3.3.9
  range-v3/0.11.0
  GENERATORS cmake_find_package
)

conan_cmake_autodetect(settings)

conan_cmake_install(
  PATH_OR_REFERENCE .
  BUILD missing
  REMOTE conan-center
  SETTINGS ${settings}
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
