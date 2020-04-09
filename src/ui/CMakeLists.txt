find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(Freetype REQUIRED)

file(GLOB_RECURSE FlowUI_Sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	 EXCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/sdl/Window.cpp)

add_library(FlowUI ${FlowUI_Sources})
target_link_libraries(FlowUI
	                  Flow
					  ${GLEW_LIBRARIES}
			          ${FREETYPE_LIBRARY})
target_include_directories(FlowUI PRIVATE ${GLEW_INCLUDE_DIRS}
	                					  ${GLM_INCLUDE_DIR}
	                					  ${FREETYPE_INCLUDE_DIRS})

