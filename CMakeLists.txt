cmake_minimum_required(VERSION 3.13)
project(ENGFLOW)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(FLOW_RESOURCES_DIR ${CMAKE_BINARY_DIR}/FlowResources/)
set(FLOW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)
file(GLOB_RECURSE HEADER_FILES ${FLOW_INCLUDE_DIR}/*.h)

include_directories(${FLOW_INCLUDE_DIR})
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	option(FLOW_BUILD_TESTING "Build Flow Testing Modules" ON)
	option(FLOW_RUN_TESTS "Run Flow Testing Modules" ON)
else()
	option(FLOW_BUILD_TESTING "Build Flow Testing Modules" OFF)
	option(FLOW_RUN_TESTS "Run Flow Testing Modules" OFF)
endif()

if(FLOW_BUILD_TESTING)
    add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif()




