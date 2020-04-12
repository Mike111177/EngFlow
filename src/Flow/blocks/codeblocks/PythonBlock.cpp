#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <Flow/blocks/codeblocks/PythonBlock.h>

struct PyRef {
	PyObject* ptr;
	PyRef() : ptr(nullptr) {}
	PyRef(PyObject* ptr) : ptr(ptr) {}
	PyRef(PyRef& copy) : ptr(copy.ptr) {
		Py_IncRef(ptr);
	}
	operator PyObject*&() {
		return ptr;
	}
	~PyRef() {
		if (ptr!=nullptr && Py_IsInitialized()) {
			Py_DecRef(ptr);
		}
	}
};

PyRef PyAdopt(PyObject* ptr) {
	Py_IncRef(ptr);
	return ptr;
}

struct Flow::PythonBlockIMPL {
	static size_t count;
	std::string name;
	PyRef pFunc = nullptr;
	size_t params;
	PythonBlockIMPL() {
		count++;
		updatePythonVMState();
	}
	void compile(std::string &source) {
		auto pModule = PyImport_ExecCodeModule("Test", PyRef(Py_CompileString(source.c_str(), "", Py_file_input)));
		auto const pDict = PyModule_GetDict(pModule);
		PyObject* pKey = nullptr, * pValue = nullptr;
		auto modname = std::string(PyModule_GetName(pModule));
		//inspect.signature
		;
		auto inspectSignature = PyAdopt(PyDict_GetItemString(PyModule_GetDict(PyRef(PyImport_ImportModule("inspect"))), "signature"));
		for (Py_ssize_t i = 0; PyDict_Next(pDict, &i, &pKey, &pValue);) {
			const char* key = PyUnicode_AsUTF8(pKey);
			if (PyFunction_Check(pValue) &&
				std::string(PyUnicode_AsUTF8(PyFunction_GetModule(pValue))) == modname) {
				pFunc = PyAdopt(pValue);//Turn Borrowed Reference to function into owned reference
				PyRef sigArgs(Py_BuildValue("(O)", pFunc));
				PyRef parameters(PyObject_GetAttrString(PyRef(PyObject_Call(inspectSignature, sigArgs, NULL)), "parameters"));
				params = PyObject_Length(parameters);
				break; //Only select first function to execute
			}
		}
	}
	~PythonBlockIMPL() { 
		count--;
		updatePythonVMState();
	}
	static void updatePythonVMState() {
		bool isInit = Py_IsInitialized();
		if (!isInit && count > 0) {
			Py_Initialize();
		} else if (isInit && count <= 0) {
			Py_Finalize();
		}
	}
};

size_t Flow::PythonBlockIMPL::count = 0;

Flow::PythonBlock::PythonBlock(std::weak_ptr<Block> p) :
	AbstractCodeBlock(p),
	impl(new PythonBlockIMPL) {

}

size_t Flow::PythonBlock::nparams() {
	return impl->params;
}

void Flow::PythonBlock::execute() {
	if (impl->pFunc == nullptr) throw "This Pythonblock is not ready yet!";
	if (nparams() > 0) throw "Python Parameters not implemented yet!";
	PyRef args(Py_BuildValue("()"));
	PyObject_Call(impl->pFunc, args, NULL); //Call function in impl
}

void Flow::PythonBlock::precompile(){
	impl->compile(source);
}

Flow::PythonBlock::~PythonBlock() {}