#include "doctest.h"

#include <filesystem>

#include <Flow/FlowDocument.h>
#include <Flow/blocks/codeblocks/JavaBlock.h>


TEST_CASE("JavaBlock common block requirements") {
	Flow::JavaBlock::reg();
	auto blockOpt = Flow::Block::create("JavaBlock");
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
	auto javaLogic = dynamic_cast<Flow::JavaBlock*>(ptr);
	SUBCASE("Block logic is actually a javablock") {
		CHECK(javaLogic != nullptr);
	}
	SUBCASE("Throws when trying to execute before supplying code") {
		CHECK_THROWS(javaLogic->execute({}));
	}
}

TEST_CASE("JavaBlock can run Java") {
	Flow::JavaBlock::reg();
	Flow::JavaBlock::setJavacLocation(JAVAC_LOC);
	auto block = Flow::Block::create("JavaBlock").value();
	auto ptr = block->logic();
	auto javaLogic = dynamic_cast<Flow::JavaBlock*>(ptr);
	SUBCASE("Runs without error") {
		REQUIRE_NOTHROW(javaLogic->setSource(
R"(
class Main {
    public static void main(String[] args) {
        System.out.println("This will be printed");
    }
}
)"));
		CHECK(javaLogic->execute({}) == "Hello Java");
	}
}