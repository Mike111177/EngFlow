#include "doctest.h"

#include <filesystem>
#include <Flow/FlowDocument.h>

TEST_CASE("FlowDocument no name errors.") {
	Flow::FlowDocument doc;
	SUBCASE("Throws when trying to open a document with no name.") {
		CHECK_THROWS(doc.open());
	}
	SUBCASE("Throws when trying to save a document with no name.") {
		CHECK_THROWS(doc.save());
	}
}

TEST_CASE("Default generated FlowDocument can actually produce an output file.") {
	auto path = std::filesystem::temp_directory_path() / "TestFlowDocument.efd";
	// If the file already exist, this test would be pretty meaningless
	if (std::filesystem::exists(path)) {
		std::filesystem::remove(path);
	}
	Flow::FlowDocument testDoc(path);
	CHECK(testDoc.save());
	bool fileExist = std::filesystem::exists(path);
	CHECK(fileExist);
	if (fileExist) {
		std::filesystem::remove(path); //Clean up
	}
}