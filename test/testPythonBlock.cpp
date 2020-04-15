#include "doctest.h"
#include <filesystem>

#include <Flow/FlowTypes.h>
#include <Flow/FlowDocument.h>
#include <Flow/CorePlugin.h>
#include <Flow/blocks/codeblocks/PythonBlock.h>


TEST_CASE("PythonBlock common block requirements") {
	initCoreComponents();
	auto blockOpt = Flow::Block::create("PythonBlock");
	SUBCASE("Block is registered") {
		CHECK(blockOpt.has_value());
	}
	auto block = *blockOpt;
	SUBCASE("Block pointer agrees with logic block ptr") {
		CHECK(block == block->logic()->block());
	}
	SUBCASE("Block logic pointer agrees with logic block logic ptr") {
		CHECK(block->logic() == block->logic()->block()->logic());
	}
	auto ptr = block->logic().get();
	auto pyLogic = dynamic_cast<Flow::PythonBlock*>(ptr);
	SUBCASE("Block logic is actually a Pythonblock") {
		CHECK(pyLogic != nullptr);
	}
	SUBCASE("Block logic pointer agrees with logic block logic ptr") {
		CHECK(block->logic() == block->logic()->block()->logic());
	}
	SUBCASE("Throws when trying to execute before supplying code") {
		CHECK_THROWS(pyLogic->execute({}));
	}
}

TEST_CASE("PythonBlock can run python") {
	initCoreComponents();
	auto block = Flow::Block::create("PythonBlock").value();
	auto ptr = block->logic().get();
	auto pyLogic = dynamic_cast<Flow::PythonBlock*>(ptr);
	SUBCASE("Runs without error") {
		pyLogic->setSource(R"(
def hello_python():
  return "Hello Python"
)");
		pyLogic->execute({});
	}
}

TEST_CASE("PythonBlock can reflect on python functions") {
	initCoreComponents();
	auto block = Flow::Block::create("PythonBlock").value();
	auto ptr = block->logic().get();
	auto pyLogic = dynamic_cast<Flow::PythonBlock*>(ptr);
	SUBCASE("Knows correct amount of parameters") {
		pyLogic->setSource(R"(
def hello_python():
  return "Hello Python"
)");
		CHECK(pyLogic->nparams() == 0);
		pyLogic->setSource(R"(
def hello_python(param1):
  return "Hello Python"
)");
		CHECK(pyLogic->nparams() == 1);
		pyLogic->setSource(R"(
def hello_python(param1, param2):
  return "Hello Python"
)");
		CHECK(pyLogic->nparams() == 2);
	}
}

TEST_CASE("PythonBlock parameter test") {
	initCoreComponents();
	auto block = Flow::Block::create("PythonBlock").value();
	auto ptr = block->logic().get();
	auto pyLogic = dynamic_cast<Flow::PythonBlock*>(ptr);
	SUBCASE("Can use parameters") {
		pyLogic->setSource(R"(
def addNumbers(a, b):
  return a + b
)");
		auto result = pyLogic->execute(Flow::Array{1, 2});
		CHECK(std::get<long>(result) == 3);
	}
	SUBCASE("Can chain parametered functions") {
		pyLogic->setSource(R"(
def doubleNum(a):
  return 2*a
)");
		auto result = pyLogic->execute(pyLogic->execute(3));
		CHECK(std::get<long>(result) == 12);
	}
}

TEST_CASE("PythonBlock can return tuple types") {
	initCoreComponents();
	auto block = Flow::Block::create("PythonBlock").value();
	auto ptr = block->logic().get();
	auto pyLogic = dynamic_cast<Flow::PythonBlock*>(ptr);
	Flow::Array input{ 1,2,3 };
	pyLogic->setSource(R"(
def testReturnTuples(p1, p2, p3):
    return p1, p2, p3
)");
	auto result = pyLogic->execute(input);
	CHECK(result == input);
}

TEST_CASE("PythonBlock saving/loading test") {
	initCoreComponents();
	std::filesystem::path path = "TestFlowPythonDocument.zip";
	// If the file already exist, this test would be pretty meaningless
	if (std::filesystem::exists(path)) { std::filesystem::remove(path); }
	CHECK(!std::filesystem::exists(path));

	Flow::FlowDocument testDoc(path);
	{
		auto block = Flow::Block::create("PythonBlock").value();
		auto pyLogic = dynamic_cast<Flow::PythonBlock*>(block->logic().get());
		auto source = R"(
def verify():
  return "I made it!"
)";
		pyLogic->setSource(source);
		block->name = "TestBlock";
		testDoc.globalBlocks.push_back(block);
	}
	testDoc.save();
	testDoc.close();
	testDoc.open();
	CHECK(testDoc.globalBlocks.size()==1);
	auto ptr = testDoc.globalBlocks[0]->logic().get();
	auto pyLogic = dynamic_cast<Flow::PythonBlock*>(ptr);
	CHECK(pyLogic != nullptr);
	auto result = pyLogic->execute({});
	CHECK(std::get<std::string>(result) == "I made it!");
}
