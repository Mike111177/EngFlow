#include "doctest.h"

#include <Flow/PluginManager.h>

TEST_CASE("Can successfully load a plugin") {
	Flow::PluginHandle testHandle(TESTPLUGIN_LOC);
	CHECK(std::string(testHandle->name()) == "TestPlugin");
}