#pragma once
#include <filesystem>
namespace Flow {
	class IPlugin {
	public:
		virtual void init() = 0;
		virtual const char* name() = 0;
	};
	class PluginDLL; //Implementation platform specific
	class PluginHandle {
		std::filesystem::path path;
		PluginDLL* dll;
		IPlugin* plugin;
	public:
		PluginHandle(std::filesystem::path path);
		~PluginHandle();
		IPlugin* operator->() { return plugin; }
	};
}