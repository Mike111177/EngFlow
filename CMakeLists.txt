cmake_minimum_required(VERSION 3.13)
project(ENGFLOW)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)



