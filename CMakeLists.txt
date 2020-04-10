cmake_minimum_required(VERSION 3.13)
project(ENGFLOW)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

set(FLOW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	option(FLOW_BUILD_TESTING "Build Flow Testing Modules" ON)
else()
	option(FLOW_BUILD_TESTING "Build Flow Testing Modules" OFF)
endif()

if(FLOW_BUILD_TESTING)
    add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif()




