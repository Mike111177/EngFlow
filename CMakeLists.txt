cmake_minimum_required(VERSION 3.13)
project(ENGFLOW)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLM REQUIRED)
find_package(Freetype REQUIRED)
find_package(pugixml REQUIRED)
find_package(MiniZip REQUIRED)
find_package(ZLIB REQUIRED)

link_libraries(${GLEW_LIBRARIES}
			   ${MINIZIP_LIBRARIES}
			   ${ZLIB_LIBRARIES}
			   ${PUGIXML_LIBRARIES}
			   ${FREETYPE_LIBRARY}
			   ${SDL2MAIN_LIBRARY}
			   ${SDL2_LIBRARY})

include_directories(${PROJECT_SOURCE_DIR}/include 
	                ${SDL2_INCLUDE_DIR}
	                ${GLEW_INCLUDE_DIRS}
	                ${GLM_INCLUDE_DIR}
	                ${FREETYPE_INCLUDE_DIRS}
	                ${PUGIXML_INCLUDE_DIR}
	                ${MINIZIP_INCLUDE_DIR})

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(EngFlow ${SRC_FILES})

