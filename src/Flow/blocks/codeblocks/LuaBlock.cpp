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
	sol::object operator()(long i, sol::state* s) { return sol::make_object(s->lua_state(), i); }
	sol::object operator()(double i, sol::state* s) { return sol::make_object(s->lua_state(), i); }
	sol::object operator()(std::string str, sol::state* s) { return sol::make_object(s->lua_state(), str); }
	template <typename T>
	sol::object operator()(T d, sol::state* s) {
		std::string tname(typeid(T).name());
		throw std::runtime_error("Flow2Lua<" + tname + "> is not implemented");
	}
} flow2lua;
sol::object Flow2Lua(Flow::FlowVar& o, sol::state& s) {
	return std::visit(flow2lua, o, std::variant<sol::state*>(&s));
}

#define RETIFAS(var, type) if (var.is<type>()) return var.as<type>()
Flow::FlowVar Lua2Flow(sol::object obj) {
	RETIFAS(obj, std::string);
	RETIFAS(obj, double);
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
				luaArgs.push_back(Flow2Lua(arg, lua));
			}
			return Lua2Flow(func(sol::as_args(luaArgs)));
		} else {
			return Lua2Flow(func(Flow2Lua(args, lua)));
		}
	}
	bool precompile(std::string const& source) {
		params.clear();
		lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);
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


