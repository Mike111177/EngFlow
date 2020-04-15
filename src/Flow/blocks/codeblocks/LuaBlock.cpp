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

const std::string Flow::LuaBlock::LogicType = "LuaBlock";

struct Flow::LuaBlockIMPL {
	lua_State* luaState;
	bool ready;
};

Flow::LuaBlock::LuaBlock(std::weak_ptr<Block> p) :
AbstractCodeBlock(p), 
impl(new LuaBlockIMPL{ luaL_newstate(), false }) {
	luaL_openlibs(impl->luaState);
	sourceExt = ".py";
}

size_t Flow::LuaBlock::nparams() {
	return 0;
}

Flow::FlowVar Flow::LuaBlock::execute(FlowVar args) {
	if (!impl->ready) throw "This Luablock is not ready yet!";
	luaL_dostring(impl->luaState, source.c_str());
}

void Flow::LuaBlock::precompile() { impl->ready = true; }

Flow::LuaBlock::~LuaBlock() {
	lua_close(impl->luaState);
}


