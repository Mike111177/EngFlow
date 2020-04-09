find_package(pugixml REQUIRED)
find_package(MiniZip REQUIRED)
find_package(ZLIB REQUIRED)
	                
add_library(Flow FlowDocument.cpp)
target_include_directories(Flow PUBLIC  ${PUGIXML_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include
	                            PRIVATE ${MINIZIP_INCLUDE_DIR})
target_link_libraries(Flow
	   				  ${MINIZIP_LIBRARIES}
			          ${ZLIB_LIBRARIES}
			          ${PUGIXML_LIBRARIES})