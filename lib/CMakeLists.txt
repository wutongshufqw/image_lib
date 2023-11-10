project(lib)

file(GLOB LIB_SRC "*.cpp")

add_library(lib ${LIB_SRC})

target_link_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
