cmake_minimum_required(VERSION 2.8.12)
project(PackageTest CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

SET(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tests
   traits.t.cpp
   type_sequence.t.cpp
   main.cpp)
target_link_libraries(tests ${CONAN_LIBS})

enable_testing()
add_test(NAME tests
   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
   COMMAND tests)
