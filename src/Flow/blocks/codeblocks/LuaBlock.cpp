#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>

#include <sol/sol.hpp>

#include <Flow/blocks/codeblocks/LuaBlock.h>

const std::string Flow::LuaBlock::LogicType = "LuaBlock";

sol::object Flow2Lua(Flow::FlowVar& o, sol::state &s);
static struct {
	//Special
	sol::object operator()(Flow::Empty m, sol::state* s) { return sol::nil; }
	sol::object operator()(int i, sol::state* s) { return sol::make_object(s->lua_state(), i); }
	template <typename T>
	sol::object operator()(T d, sol::state* s) {
		throw std::runtime_error(std::string("Flow2Lua<") + std::string(typeid(T).name()) + std::string("> is not implemented"));
	}
} flow2lua;
using StateV = std::variant<sol::state*>;
sol::object Flow2Lua(Flow::FlowVar& o, StateV s) {
	return std::visit(flow2lua, o, s);
}

#define RETIFAS(var, type) if (var.is<type>()) return var.as<type>()
Flow::FlowVar Lua2Flow(sol::object obj) {
	RETIFAS(obj, std::string);
	RETIFAS(obj, int);
	return {};
}
#undef RETIFAS

struct Flow::LuaBlock::IMPL {
	sol::state lua;
	sol::function func;
	std::vector<std::string> params;
	std::vector<std::string> nparams() {
		return params;
	}
	FlowVar run(FlowVar args) {
		if (std::holds_alternative<Empty>(args)) {
			return Lua2Flow(func());
		} else if (std::holds_alternative<Array>(args)) {
			std::vector<sol::object> luaArgs;
			for (auto &arg : std::get<Array>(args)) {
				luaArgs.push_back(Flow2Lua(arg, &lua));
			}
			return Lua2Flow(func(sol::as_args(luaArgs)));
		} else {
			return Lua2Flow(func(Flow2Lua(args, &lua)));
		}
	}
	bool precompile(std::string const& source) {
		params.clear();
		sol::object obj = lua.do_string(source);
		if (obj.get_type() == sol::type::table) {
			auto tab = obj.as<sol::table>();
			func = tab["exe"];
		} else if (obj.get_type() == sol::type::function) {
			auto fn = obj.as<sol::function>();
			func = fn;
		} else {
			throw std::runtime_error("No function found in code block.");
		}
		return true;
	}
};

//Pimpl forward
Flow::LuaBlock::LuaBlock(std::weak_ptr<Block> p) : AbstractCodeBlock(p), impl(new IMPL) {sourceExt = ".lua";}
std::vector<std::string> Flow::LuaBlock::nparams(){return impl->nparams();}
Flow::FlowVar Flow::LuaBlock::run(FlowVar args) {return impl->run(args);}
bool Flow::LuaBlock::precompile() { return impl->precompile(source); }
Flow::LuaBlock::~LuaBlock() {}


