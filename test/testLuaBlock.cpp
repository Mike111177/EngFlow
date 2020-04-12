#include "doctest.h"

#include <Flow/blocks/codeblocks/LuaBlock.h>


TEST_CASE("LuaBlock common block requirements") {
	auto block = Flow::Block::create<Flow::LuaBlock>();
	SUBCASE("Block pointer agrees with logic block ptr") {
		CHECK(block == block->logic()->block());
	}
	SUBCASE("Block logic pointer agrees with logic block logic ptr") {
		CHECK(block->logic() == block->logic()->block()->logic());
	}
	auto ptr = block->logic().get();
	auto luaLogic = dynamic_cast<Flow::LuaBlock*>(ptr);
	SUBCASE("Block logic is actually a luablock") {
		CHECK(luaLogic != nullptr);
	}
	SUBCASE("Throws when trying to execute before supplying code") {
		CHECK_THROWS(luaLogic->execute());
	}
}

TEST_CASE("LuaBlock can run Lua") {
	auto block = Flow::Block::create<Flow::LuaBlock>();
	auto ptr = block->logic().get();
	auto luaLogic = dynamic_cast<Flow::LuaBlock*>(ptr);
	SUBCASE("Runs without error") {
		luaLogic->setSource("print(\"Hello LuaBlock\")");
		luaLogic->execute();
	}
}



