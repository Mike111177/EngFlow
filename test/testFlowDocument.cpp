#include "doctest.h"

#include <filesystem>
#include <Flow/FlowDocument.h>

TEST_CASE("FlowDocument file path errors.") {
	Flow::FlowDocument doc;
	SUBCASE("Throws when trying to open a document with no name.") {
		CHECK_THROWS(doc.open());
	}
	SUBCASE("Throws when trying to save a document with no name.") {
		CHECK_THROWS(doc.save());
	}
}

TEST_CASE("Default generated FlowDocument can actually produce a readable output file.") {
	auto path = std::filesystem::temp_directory_path() / "TestFlowDocument.efd";
	// If the file already exist, this test would be pretty meaningless
	if (std::filesystem::exists(path)) {std::filesystem::remove(path);}
	CHECK(!std::filesystem::exists(path));

	Flow::FlowDocument testDoc(path);
	CHECK(testDoc.save());
	testDoc.close();
	CHECK(std::filesystem::exists(path));
	CHECK(testDoc.open());
	testDoc.close();
}