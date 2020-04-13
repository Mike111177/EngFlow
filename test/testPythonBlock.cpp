#include "doctest.h"

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
  print("Hello Python")
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
  print("Hello Python")
)");
		CHECK(pyLogic->nparams() == 0);
		pyLogic->setSource(R"(
def hello_python(param1):
  print("Hello Python")
)");
		CHECK(pyLogic->nparams() == 1);
		pyLogic->setSource(R"(
def hello_python(param1, param2):
  print("Hello Python")
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
		auto result = pyLogic->execute({1,2});
		CHECK(std::get<long>(result) == 3);
	}
	SUBCASE("Can chain parametered functions") {
		pyLogic->setSource(R"(
def doubleNum(a):
  return 2*a
)");
		auto result = pyLogic->execute({ pyLogic->execute({ 3 }) });
		CHECK(std::get<long>(result) == 12);
	}
}
