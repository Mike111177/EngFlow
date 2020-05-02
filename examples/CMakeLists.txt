if (FLOW_BUILD_LUABLOCK AND FLOW_BUILD_PYTHONBLOCK)
	add_executable(example_Python_Lua_Interop PythonLuaInterop.cpp)
	target_link_libraries(example_Python_Lua_Interop Flow PythonBlock LuaBlock)
endif()