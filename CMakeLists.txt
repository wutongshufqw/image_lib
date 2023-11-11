cmake_minimum_required(VERSION 3.0)
project(Image VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

add_executable(main main.cpp)

target_link_libraries(main lib)

set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

add_compile_options(-Wall -g)
if(MSVC)
    target_compile_options(main PRIVATE -O2 "/utf-8")
else()
    target_compile_options(main PRIVATE -g -O2)
endif()
