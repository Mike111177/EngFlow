#include <iostream>
#include <string>

// include Lua, assumes it is local to this file
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

int testLua() {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	const char* code = "print(\"Hello World\")";
	luaL_dostring(L, code);
	lua_close(L);
	return 0;
}