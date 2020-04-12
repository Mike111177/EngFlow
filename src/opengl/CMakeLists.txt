find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(Freetype REQUIRED)

file(GLOB_RECURSE FLOWGL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(FlowGL ${FLOWGL_SOURCES})
target_link_libraries(FlowGL ${GLEW_LIBRARIES} ${FREETYPE_LIBRARY})
target_include_directories(FlowGL PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
										  ${GLEW_INCLUDE_DIRS}
	                					  ${GLM_INCLUDE_DIR}
	                					  ${FREETYPE_INCLUDE_DIRS})