#include <Flow/PluginManager.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
class Flow::PluginDLL {
	typedef IPlugin* (*CFUNC)();
	typedef void (*DFUNC)(IPlugin*);
	HINSTANCE dll;
	CFUNC creator;
	DFUNC destroyer;
public:
	PluginDLL(const char* path) {
		dll = LoadLibrary(path);
		creator = (CFUNC)GetProcAddress(dll, "CreatePlugin");
		destroyer = (DFUNC)GetProcAddress(dll, "DestroyPlugin");
	}
	auto create() {
		return creator();
	}
	auto destroy(IPlugin* instance) {
		return destroyer(instance);
	}
};
#endif

Flow::PluginHandle::PluginHandle(std::filesystem::path path) :
	path(path),
	dll(new PluginDLL(path.string().c_str())),
	plugin(dll->create()){
	plugin->init();
}

Flow::PluginHandle::~PluginHandle() {}
