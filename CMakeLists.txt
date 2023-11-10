cmake_minimum_required(VERSION 3.0)
project(Image VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

add_executable(main main.cpp)

target_link_libraries(main lib)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -g)
target_compile_options(main PRIVATE -g -O2)
