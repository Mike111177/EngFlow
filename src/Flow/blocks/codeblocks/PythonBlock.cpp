#include <vector>
#include <sstream>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <Flow/blocks/codeblocks/PythonBlock.h>

const std::string Flow::PythonBlock::LogicType = "PythonBlock";

void PrintPy(PyObject* p) {
	std::cout << PyUnicode_AsUTF8(PyObject_Repr(p)) << std::endl;
}

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
			//Py_DecRef(ptr);
		}
	}
};

PyRef PyAdopt(PyObject* ptr) {
	Py_IncRef(ptr);
	return ptr;
}

struct Flow2Py {
	PyRef operator()(std::monostate m) { return PyAdopt(Py_None); }
	PyRef operator()(long i) { return PyLong_FromLong(i); }
	PyRef operator()(std::string s) { return PyUnicode_FromStringAndSize(s.c_str(), s.size()); }
};

Flow::FlowVar Py2Flow(PyObject* p) {
	if (PyLong_Check(p)) {
		return PyLong_AsLong(p);
	} else if (PyUnicode_Check(p)) {
		auto size = PyUnicode_GetSize(p);
		return std::string(PyUnicode_AsUTF8(p), size);
	}
	return std::monostate();
}

template<class C>
PyRef toTuple(C d) {
	auto s = d.size();
	auto tup = PyTuple_New(s);
	for (int i = 0; i < s; i++) {
		auto o = std::visit(Flow2Py(), d[i]);
		PyTuple_SET_ITEM(tup, i, o);
	}
	return tup;
}

PyObject* CatchPy(PyObject* p) {
	PyObject* err;
	if (p == nullptr && (err = PyErr_Occurred()) != nullptr) {
		//PyObject* type, * value, * traceback;
		//PyErr_Fetch(&type, &value, &traceback);
		//PrintPy(err);
		//auto e = PyObject_CallObject(type, value);
		//PrintPy(e);
		//PrintPy(PyObject_GetAttrString(e, "filename"));
		PyErr_Print();
		throw "Python Error";
	}
	return p;
}

struct Flow::PythonBlockIMPL {
	static size_t count;
	PyRef pFunc = nullptr;
	size_t params;
	PythonBlockIMPL() {
		count++;
		updatePythonVMState();
	}
	void compile(std::string const &source, std::string const& name) {
		PyRef byteCode(CatchPy(Py_CompileString(source.c_str(), name.c_str(), Py_file_input)));
		auto pModule = PyImport_ExecCodeModule(name.c_str(), byteCode);
		auto modname = std::string(PyModule_GetName(pModule));
		//inspect.signature
		auto inspectSignature = PyAdopt(PyDict_GetItemString(PyModule_GetDict(PyRef(PyImport_ImportModule("inspect"))), "signature"));

		auto const pDict = PyModule_GetDict(pModule);
		PyObject* pKey = nullptr, * pValue = nullptr;
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
		if (pFunc == nullptr) {
			throw "Python was unable find a function in this code";
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
	sourceExt = ".py";
}

size_t Flow::PythonBlock::nparams() {
	return impl->params;
}

Flow::FlowVar Flow::PythonBlock::execute(std::vector<FlowVar> args) {
	if (impl->pFunc == nullptr) throw "This Pythonblock is not ready yet!";
	return Py2Flow(PyRef(PyObject_Call(impl->pFunc, toTuple(args), NULL))); //Call function in impl
}

void Flow::PythonBlock::precompile(){
	impl->compile(source,block()->name);
}

Flow::PythonBlock::~PythonBlock() {}