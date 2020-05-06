#include "doctest.h"

#include <filesystem>

#include <Flow/FlowDocument.h>
#include <Flow/blocks/codeblocks/LuaBlock.h>


TEST_CASE("LuaBlock common block requirements") {
	Flow::LuaBlock::reg();
	auto blockOpt = Flow::Block::create("LuaBlock");
	SUBCASE("Block is registered") {
		CHECK(blockOpt.has_value());
	}
	auto block = blockOpt.value();
	SUBCASE("Block pointer agrees with logic block ptr") {
		CHECK(block.get() == &block->logic()->block());
	}
	SUBCASE("Block logic pointer agrees with logic block logic ptr") {
		CHECK(block->logic() == block->logic()->block().logic());
	}
	auto ptr = block->logic();
	auto luaLogic = dynamic_cast<Flow::LuaBlock*>(ptr);
	SUBCASE("Block logic is actually a luablock") {
		CHECK(luaLogic != nullptr);
	}
	SUBCASE("Throws when trying to execute before supplying code") {
		CHECK_THROWS(luaLogic->execute({}));
	}
}

TEST_CASE("LuaBlock can run Lua") {
	Flow::LuaBlock::reg();
	auto block = Flow::Block::create("LuaBlock").value();
	auto ptr = block->logic();
	auto luaLogic = dynamic_cast<Flow::LuaBlock*>(ptr);
	SUBCASE("Runs without error") {
	REQUIRE_NOTHROW(luaLogic->setSource(
R"(
function hellolua()
     return "Hello Lua"
end
return hellolua
)"));
	CHECK(luaLogic->execute({}) == "Hello Lua");
	}
}

TEST_CASE("LuaBlock parameter test") {
	Flow::LuaBlock::reg();
	auto block = Flow::Block::create("LuaBlock").value();
	auto ptr = block->logic();
	auto luaLogic = dynamic_cast<Flow::LuaBlock*>(ptr);

	luaLogic->setSource(
R"(
function addNumbers(a, b)
  return a + b
end
return addNumbers
)");
	Flow::FlowVar result = luaLogic->execute(Flow::Array{ 1, 2 });
	CHECK(result == 3);

	auto numtodouble = 3;
	luaLogic->setSource(
R"(
function doubleNum(a)
  return 2*a
end
return doubleNum
)");
	REQUIRE_NOTHROW(result = luaLogic->execute(numtodouble));
	CHECK(result == numtodouble * 2);
	REQUIRE_NOTHROW(result = luaLogic->execute(result));
	CHECK(result == numtodouble * 2 * 2);
}

TEST_CASE("LuaBlock saving/loading test") {
	Flow::LuaBlock::reg();
	auto path = std::filesystem::temp_directory_path() / "TestFlowLuaDocument.efd";
	// If the file already exist, this test would be pretty meaningless
	if (std::filesystem::exists(path)) { try { std::filesystem::remove(path); } catch (...) {} }
	REQUIRE_FALSE(std::filesystem::exists(path));
	{ //Scoped to ensure no carry-over in memory
		Flow::FlowDocument testDoc(path);
		auto block = Flow::Block::create("LuaBlock").value();
		auto luaLogic = dynamic_cast<Flow::LuaBlock*>(block->logic());
		luaLogic->setSource(
			R"(
function verify()
  return "I made it!"
end
return verify
)");
		CHECK(luaLogic->execute({}) == "I made it!"); //Make sure it works before saving it
		block->name = "TestBlock";
		testDoc.globalBlocks.push_back(block);
		testDoc.save();
		testDoc.close();
	}
	REQUIRE_MESSAGE(std::filesystem::exists(path), "Could not create file, unknown reason.");
	{ //Now to load
		Flow::FlowDocument testDoc(path);
		testDoc.open();
		CHECK(testDoc.globalBlocks.size() == 1); //We saved one block, we should recieve one block
		//Any Code Block should behave the same, assuming the output is correct
		auto luaLogic = dynamic_cast<Flow::AbstractCodeBlock*>(testDoc.globalBlocks[0]->logic());
		REQUIRE_MESSAGE(luaLogic != nullptr, "Could not convert the only block in this file to a code block.");
		CHECK(luaLogic->execute({}) == "I made it!");
	}
	//Cleanup
	if (std::filesystem::exists(path)) {
		try { std::filesystem::remove(path); } catch (...) {}
	}
	//WARN_FALSE_MESSAGE(std::filesystem::exists(path), "Unable to clean up test file");
}

TEST_CASE("Multiple LuaBlocks can communicate ") {
	Flow::LuaBlock::reg();
	auto block1 = Flow::Block::create("LuaBlock").value();
	auto ptr1 = block1->logic();
	auto luaLogic1 = dynamic_cast<Flow::LuaBlock*>(ptr1);
	REQUIRE_NOTHROW(luaLogic1->setSource(
R"(
function retString()
  return "Hello Lua"
end
return retString
)"));
	auto block2 = Flow::Block::create("LuaBlock").value();
	auto ptr2 = block2->logic();
	auto luaLogic2 = dynamic_cast<Flow::LuaBlock*>(ptr2);
	REQUIRE_NOTHROW(luaLogic2->setSource(
R"(
function splitString(the_string)
  return string.sub(the_string, 1, 5)
end
return splitString
)"));
	CHECK(luaLogic2->execute(luaLogic1->execute({})) == "Hello");
}



