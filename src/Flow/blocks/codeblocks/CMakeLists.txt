set(FLOW_CODEBLOCKS_HEADER_DIR ${PROJECT_SOURCE_DIR}/include/Flow/blocks/codeblocks/)
# List of all blocks

# Lua Block
option(FLOW_BUILD_LUABLOCK "Build Lua Programming Block" ON)
if (${FLOW_BUILD_LUABLOCK})
	find_package(Lua)

	add_library(LuaBlock LuaBlock.cpp ${FLOW_CODEBLOCKS_HEADER_DIR}/LuaBlock.h)
	
	target_include_directories(LuaBlock PRIVATE ${FLOW_INCLUDE_DIR} ${LUA_INCLUDE_DIR})
	target_link_libraries(LuaBlock ${LUA_LIBRARY})
	
	list(APPEND FLOW_BUILT_BLOCKS LuaBlock)
endif()