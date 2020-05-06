#include <vector>
#include <sstream>
#include <concepts>
#include <thread>

#define Py_DEBUG
//#define Py_TRACE_REFS
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <Flow/Util.h>
#include <Flow/blocks/codeblocks/PythonBlock.h>

const std::string Flow::PythonBlock::LogicType = "PythonBlock";

class PyRef {
	PyObject* ptr;
	void inc() {
		if (ptr != nullptr && Py_IsInitialized()) { Py_IncRef(ptr); }
	}
	void dec() {
		if (ptr != nullptr && Py_IsInitialized() && PyGILState_Check() && ptr->ob_refcnt>0) { Py_CLEAR(ptr); }
	}
public:
	static PyRef adopt(PyObject* ptr){
		Py_IncRef(ptr);
		return ptr;
	}
	PyRef() : ptr(nullptr) {}
	PyRef(PyObject* ptr) : ptr(ptr) {}
	PyRef(PyRef const& copy) : ptr(copy.ptr) {inc();}
	~PyRef() { dec(); }
	PyRef& operator=(PyObject*& nptr) {
		dec();
		ptr = nptr;
		return *this;
	}
	operator PyObject* () { return ptr; }
	void clear() {
		dec();
		ptr = nullptr;
	}

};

class PyState {
	PyThreadState* s;
public:
	class lock {
		PyGILState_STATE gs;
	public:
		lock() : gs(PyGILState_Ensure()) {
		}
		~lock () {
			PyGILState_Release(gs); 
		}
	};
	PyState() {
		Py_Initialize();
		PyEval_InitThreads();
		s = PyEval_SaveThread();
	}
	~PyState() { 
		PyEval_RestoreThread(s);
		Py_Finalize();
	}
};



void PrintPy(PyObject* p) {
	std::cout << PyUnicode_AsUTF8(PyObject_Repr(p)) << std::endl;
}

PyObject* Flow2Py(Flow::FlowVar& o);
static struct {
	//Special
	PyObject* operator()(Flow::Empty m) { return Py_None; }
	PyObject* operator()(Flow::Null m) { return Py_None; }
	//String
	PyObject* operator()(std::string s) { return PyUnicode_FromStringAndSize(s.c_str(), s.size()); }
	//Numbers
	template <std::signed_integral T>
	PyObject* operator()(T i) { return PyLong_FromLong(i); }
	template <std::unsigned_integral T>
	PyObject* operator()(T ui) { return PyLong_FromUnsignedLong(ui); }
	template <std::floating_point T>
	PyObject* operator()(T d) { return PyFloat_FromDouble(d); }
	//Arrays
	PyObject* operator()(Flow::Array d) {
		auto s = d.size();
		auto tup = PyTuple_New(s);
		for (int i = 0; i < s; i++) {
			auto o = Flow2Py(d[i]);
			PyTuple_SET_ITEM(tup, i, o);
		}
		return tup;
	}
	//Dicts
	PyObject* operator()(Flow::Dict d) {
		auto dict = PyDict_New();
		for (auto& fo : d) PyDict_SetItemString(dict, fo.first.c_str(), Flow2Py(fo.second));
		return dict;
	}
} flow2py;

PyObject* Flow2Py(Flow::FlowVar &o) {
	return std::visit(flow2py, o);
}

Flow::FlowVar Py2Flow(PyObject* p) {
	if (PyLong_Check(p)) {
		return PyLong_AsLong(p);
	} else if (PyFloat_Check(p)) {
		return PyFloat_AsDouble(p);
	} else if (PyUnicode_Check(p)) {
		auto size = PyUnicode_GET_LENGTH(p);
		return Flow::String(PyUnicode_AsUTF8(p), size);
	} else if (PyTuple_Check(p)) {
		auto size = PyTuple_Size(p);
		Flow::Array fTup(size);
		for (auto i = 0; i < size; i++) {
			//Circular references will break this boy
			fTup[i] = Py2Flow(PyTuple_GetItem(p, i));
		}
		return fTup;
	} else if (PyDict_Check(p)) {
		Flow::Dict fDict;
		PyObject* pKey = nullptr, * pValue = nullptr;
		for (Py_ssize_t i = 0; PyDict_Next(p, &i, &pKey, &pValue);) {
			fDict.insert({ PyUnicode_AsUTF8(pKey), Py2Flow(pValue) });
		}
		return fDict;
	}
	return std::monostate();
}

PyObject* CatchPy(PyObject* p) {
	PyObject* err;
	if (p == nullptr && (err = PyErr_Occurred()) != nullptr) {
		PyObject* type, * value, * traceback;
		PyErr_Fetch(&type, &value, &traceback);
		auto pstr = PyObject_Str(value);
		std::string errstr(PyUnicode_AsUTF8(pstr));
		throw std::runtime_error("Python Error: " + errstr);
	}
	return p;
}

static size_t nextUID = 0;

struct Flow::PythonBlockIMPL {
	std::vector<std::string> params;
	static_ptr<PythonBlockIMPL, PyState> state;
	PyObject* pModule;
	std::string internal_name;
	PyObject* pFunc = nullptr;
	bool loaded = false;
	/*
	struct PyModuleDef def = {
		PyModuleDef_HEAD_INIT, 
		nullptr, 
		NULL,
		-1, 
		{}
	};*/
	bool compile(std::string const &source, std::string const& name) {
		PyObject* byteCode = Py_CompileString(source.c_str(), name.c_str(), Py_file_input);
		internal_name = name + "PyBlock_" + std::to_string(nextUID++);
		//def.m_name = internal_name.c_str();
		//PyModuleDef_Init(&def);
		pModule = PyModule_New(internal_name.c_str());
		loaded = true;

		auto pModDict = CatchPy(PyModule_GetDict(pModule));
		PyObject* builtins = PyEval_GetBuiltins();
		PyDict_SetItemString(pModDict, "__builtins__", builtins);
		PyEval_EvalCode(byteCode, pModDict, nullptr);
		auto modname = std::string(PyModule_GetName(pModule));
		auto inspectFunc = PyRef::adopt(PyDict_GetItemString(PyModule_GetDict(PyRef(PyImport_ImportModule("inspect"))), "signature"));

		auto const pDict = PyModule_GetDict(pModule);
		PyObject* pKey = nullptr, * pValue = nullptr;
		for (Py_ssize_t i = 0; PyDict_Next(pDict, &i, &pKey, &pValue);) {
			const char* key = PyUnicode_AsUTF8(pKey);
			if (PyFunction_Check(pValue) &&
				std::string(PyUnicode_AsUTF8(PyFunction_GetModule(pValue))) == modname) {
				pFunc = pValue;//Turn Borrowed Reference to function into owned reference
				PyRef sigArgs(Py_BuildValue("(O)", pFunc));
				PyRef sizRes(PyObject_Call(inspectFunc, sigArgs, NULL)); //run signature function on function
				PyRef parameters(PyObject_GetAttrString(sizRes, "parameters"));
				params.clear();
				if (PyMapping_Check(parameters)) {
					auto paramKeys = PyMapping_Keys(parameters);
					auto size = PyList_Size(paramKeys);
					for (auto i = 0; i < size; i++) {
						auto paramKey = PyList_GetItem(paramKeys, i);
						params.emplace_back(PyUnicode_AsUTF8(paramKey));
					}
				}
				break; //Only select first function to execute
			}
		}
		if (pFunc == nullptr) {
			throw std::runtime_error("Python was unable find a function in this code");
		}
		return true;
	}
};

Flow::PythonBlock::PythonBlock(std::weak_ptr<Block> p) :
	AbstractCodeBlock(p),
	impl(new PythonBlockIMPL) {
	sourceExt = ".py";
}

std::vector<std::string> Flow::PythonBlock::nparams() {
	return impl->params;
}

Flow::FlowVar Flow::PythonBlock::run(FlowVar args) {
	if (impl->pFunc == nullptr) throw "This Pythonblock is not ready yet!";
	if (!std::holds_alternative<Array>(args)) {
		Array newArgs;
		if (!std::holds_alternative<Flow::Empty>(args)) { //We want an empty tuple if it is delibritley empty
			newArgs.push_back(args);
		}
		args = FlowVar(newArgs);
	}
	PyState::lock l;
	auto pyArg = CatchPy(Flow2Py(args));
	auto pyResult = CatchPy(PyObject_Call(impl->pFunc, pyArg, NULL));
	auto result = Py2Flow(pyResult); 
	return result;
}

void Flow::PythonBlock::reg() { Block::registerLogic<PythonBlock>(); }

bool Flow::PythonBlock::precompile(){
	PyState::lock l;
	return impl->compile(source,block().name);
}

Flow::PythonBlock::~PythonBlock() {}