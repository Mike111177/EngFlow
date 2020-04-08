cmake_minimum_required(VERSION 3.13)
project(ENGFLOW)
set(CMAKE_CXX_STANDARD 20)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(EngFlow ${SRC_FILES})

