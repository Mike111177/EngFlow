#pragma once
#include <filesystem>
namespace Flow {
	class IPlugin {
		IPlugin(IPlugin&) = delete;
	protected:
		IPlugin() = default;
	public:
		virtual void init() = 0;
		virtual const char* name() = 0;
	};
	class PluginDLL; //Implementation platform specific
	class PluginHandle {
		std::filesystem::path path;
		PluginDLL* dll;
		IPlugin* plugin;
		PluginHandle(PluginHandle&) = delete;
		PluginHandle() = delete;
	public:
		PluginHandle(std::filesystem::path path);
		~PluginHandle();
		IPlugin* operator->();
		IPlugin* operator*();

	};
}