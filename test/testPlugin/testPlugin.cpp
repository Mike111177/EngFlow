#include <iostream>

#include "flowplugintest_export.h"

#include <Flow/PluginManager.h>

class TestPlugin : public Flow::IPlugin {
	virtual void init() override {}
	virtual const char* name() override { return "TestPlugin"; }
};
extern "C" {
	FLOWPLUGINTEST_EXPORT Flow::IPlugin* CreatePlugin() {
		return new TestPlugin;
	}
}