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
	REQUIRE_NOTHROW(pyLogic->setSource(
R"(
def hello_python():
  return "Hello Python"
)"));
	CHECK(pyLogic->execute({}) == "Hello Python");
}

TEST_CASE("PythonBlock knows how many parameters to pass its function") {
	initCoreComponents();
	auto block = Flow::Block::create("PythonBlock").value();
	auto ptr = block->logic().get();
	auto pyLogic = dynamic_cast<Flow::PythonBlock*>(ptr);
	REQUIRE_NOTHROW(pyLogic->setSource(
R"(
def hello_python():
  return "Hello Python"
)"));
	CHECK(pyLogic->nparams() == 0);
	REQUIRE_NOTHROW(pyLogic->setSource(
R"(
def hello_python(param1):
  return "Hello Python"
)"));
	CHECK(pyLogic->nparams() == 1);
	REQUIRE_NOTHROW(pyLogic->setSource(
R"(
def hello_python(param1, param2):
  return "Hello Python"
)"));
	CHECK(pyLogic->nparams() == 2);
}

TEST_CASE("PythonBlock parameter test") {
	initCoreComponents();
	auto block = Flow::Block::create("PythonBlock").value();
	auto ptr = block->logic().get();
	auto pyLogic = dynamic_cast<Flow::PythonBlock*>(ptr);

	pyLogic->setSource(R"(
def addNumbers(a, b):
  return a + b
)");
	Flow::FlowVar result = pyLogic->execute(Flow::Array{1, 2});
	CHECK(result == 3);

	auto numtodouble = 3;
	pyLogic->setSource(
R"(
def doubleNum(a):
  return 2*a
)");
	REQUIRE_NOTHROW(result = pyLogic->execute(numtodouble));
	CHECK(result == numtodouble * 2);
	REQUIRE_NOTHROW(result = pyLogic->execute(result));
	CHECK(result == numtodouble * 2 * 2);
}

TEST_CASE("PythonBlock can return tuple types") {
	initCoreComponents();
	auto block = Flow::Block::create("PythonBlock").value();
	auto ptr = block->logic().get();
	auto pyLogic = dynamic_cast<Flow::PythonBlock*>(ptr);
	Flow::Array input{ 1,2,3 };
	REQUIRE_NOTHROW(pyLogic->setSource(
R"(
def testReturnTuples(p1, p2, p3):
    return p1, p2, p3
)"));
	Flow::FlowVar result;
	REQUIRE_NOTHROW(result = pyLogic->execute(input));
	CHECK(result == input);
}

TEST_CASE("PythonBlock saving/loading test") {
	initCoreComponents();
	auto path = std::filesystem::temp_directory_path()/"TestFlowPythonDocument.efd";
	// If the file already exist, this test would be pretty meaningless
	if (std::filesystem::exists(path)) { try { std::filesystem::remove(path); } catch (...) {} }
	REQUIRE_FALSE(std::filesystem::exists(path));
	{ //Scoped to ensure no carry-over in memory
		Flow::FlowDocument testDoc(path);
		auto block = Flow::Block::create("PythonBlock").value();
		auto pyLogic = dynamic_cast<Flow::PythonBlock*>(block->logic().get());
		pyLogic->setSource(
R"(
def verify():
  return "I made it!"
)");
		CHECK(pyLogic->execute({}) == "I made it!"); //Make sure it works before saving it
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
		auto pyLogic = dynamic_cast<Flow::AbstractCodeBlock*>(testDoc.globalBlocks[0]->logic().get());
		REQUIRE_MESSAGE(pyLogic != nullptr, "Could not convert the only block in this file to a code block.");
		CHECK(pyLogic->execute({}) == "I made it!");
	}
	//Cleanup
	if (std::filesystem::exists(path)) { 
		try {std::filesystem::remove(path);}catch (...){}
	}
	WARN_FALSE_MESSAGE(std::filesystem::exists(path), "Unable to clean up test file");
}
