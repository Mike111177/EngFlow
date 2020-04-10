find_package(pugixml REQUIRED)
find_package(MiniZip REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB_RECURSE FLOW_HEADERS ${PROJECT_SOURCE_DIR}/include/Flow/*.h)
file(GLOB FLOW_SOURCE *.cpp)
	                
add_library(Flow ${FLOW_SOURCE} ${CMAKE_CURRENT_SOURCE_DIR}/blocks/AbstractCodeBlock.cpp ${FLOW_HEADERS})
target_include_directories(Flow PUBLIC  ${PUGIXML_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include
	                            PRIVATE ${MINIZIP_INCLUDE_DIR})
target_link_libraries(Flow
	   				  ${MINIZIP_LIBRARIES}
			          ${ZLIB_LIBRARIES}
			          ${PUGIXML_LIBRARIES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/blocks/codeblocks)