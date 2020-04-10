#include <iostream>
#include <string>

// include Lua, assumes it is local to this file
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <Flow/blocks/codeblocks/LuaBlock.h>

struct Flow::LuaBlockIMPL {
	lua_State* luaState;
};

Flow::LuaBlock::LuaBlock(std::weak_ptr<Block> p) :
AbstractCodeBlock(p), 
impl(new LuaBlockIMPL{ luaL_newstate() }) {
	luaL_openlibs(impl->luaState);
}

void Flow::LuaBlock::execute() {
	luaL_dostring(impl->luaState, source.c_str());
	
}

void Flow::LuaBlock::precompile() {}

Flow::LuaBlock::~LuaBlock() {
	lua_close(impl->luaState);
}


