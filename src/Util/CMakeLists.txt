file(GLOB_RECURSE FLOW_UTIL_HEADERS ${PROJECT_SOURCE_DIR}/include/Util/*.h)
file(GLOB FLOW_UTIL_SOURCE *.cpp)
	                
add_library(FlowUtil ${FLOW_UTIL_SOURCE} ${FLOW_UTIL_HEADERS})