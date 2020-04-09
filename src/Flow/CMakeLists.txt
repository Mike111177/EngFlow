find_package(pugixml REQUIRED)
find_package(MiniZip REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB_RECURSE FLOW_HEADERS ${PROJECT_SOURCE_DIR}/include/Flow/*.h)
	                
add_library(Flow FlowDocument.cpp ${FLOW_HEADERS})
target_include_directories(Flow PUBLIC  ${PUGIXML_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include
	                            PRIVATE ${MINIZIP_INCLUDE_DIR})
target_link_libraries(Flow
	   				  ${MINIZIP_LIBRARIES}
			          ${ZLIB_LIBRARIES}
			          ${PUGIXML_LIBRARIES})