set(FLOW_CODEBLOCKS_HEADER_DIR ${PROJECT_SOURCE_DIR}/include/Flow/blocks/codeblocks/)
# List of all blocks

# Lua Block
option(FLOW_BUILD_LUABLOCK "Build Lua Programming Block" ON)
if (${FLOW_BUILD_LUABLOCK})
	find_package(Lua REQUIRED)
	find_package(Sol2 REQUIRED)

	add_library(LuaBlock LuaBlock.cpp ${FLOW_CODEBLOCKS_HEADER_DIR}/LuaBlock.h)
	
	target_include_directories(LuaBlock PRIVATE ${FLOW_INCLUDE_DIR} ${LUA_INCLUDE_DIR}  ${SOL2_INCLUDE_DIRS})
	target_link_libraries(LuaBlock ${LUA_LIBRARY})
	
	list(APPEND FLOW_BUILT_BLOCKS LuaBlock)
endif()

# Python Block
option(FLOW_BUILD_PYTHONBLOCK "Build Python Programming Block" ON)
if (${FLOW_BUILD_PYTHONBLOCK})
	find_package(Python3 REQUIRED 
		         COMPONENTS Interpreter Development)

	add_library(PythonBlock PythonBlock.cpp ${FLOW_CODEBLOCKS_HEADER_DIR}/PythonBlock.h)
	target_include_directories(PythonBlock PRIVATE ${FLOW_INCLUDE_DIR} ${Python3_INCLUDE_DIRS})
	target_link_libraries(PythonBlock ${Python3_LIBRARIES})
	
	list(APPEND FLOW_BUILT_BLOCKS PythonBlock)
endif()

# Java Block
option(FLOW_BUILD_JAVABLOCK "Build Java Programming Block" ON)
if (${FLOW_BUILD_JAVABLOCK})
	find_package(JNI REQUIRED)

	add_library(JavaBlock JavaBlock.cpp ${FLOW_CODEBLOCKS_HEADER_DIR}/JavaBlock.h)
	target_include_directories(JavaBlock PRIVATE ${FLOW_INCLUDE_DIR} ${JNI_INCLUDE_DIRS})
	target_link_libraries(JavaBlock ${JNI_LIBRARIES})
	
	list(APPEND FLOW_BUILT_BLOCKS JavaBlock)
endif()