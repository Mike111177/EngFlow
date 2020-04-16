#include <vector>
#include <sstream>
#include <concepts>

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

PyRef Flow2Py(Flow::FlowVar& o);
static struct {
	//Special
	PyRef operator()(Flow::Empty m) { return PyAdopt(Py_None); }
	PyRef operator()(Flow::Null m) { return PyAdopt(Py_None); }
	//String
	PyRef operator()(std::string s) { return PyUnicode_FromStringAndSize(s.c_str(), s.size()); }
	//Numbers
	template <std::signed_integral T>
	PyRef operator()(T i) { return PyLong_FromLong(i); }
	template <std::unsigned_integral T>
	PyRef operator()(T ui) { return PyLong_FromUnsignedLong(ui); }
	template <std::floating_point T>
	PyRef operator()(T d) { return PyFloat_FromDouble(d); }
	//Arrays
	PyRef operator()(Flow::Array d) {
		auto s = d.size();
		auto tup = PyTuple_New(s);
		for (int i = 0; i < s; i++) {
			auto o = Flow2Py(d[i]);
			PyTuple_SET_ITEM(tup, i, o);
		}
		return tup;
	}
} flow2py;

PyRef Flow2Py(Flow::FlowVar &o) {
	return std::visit(flow2py, o);
}

Flow::FlowVar Py2Flow(PyObject* p) {
	if (PyLong_Check(p)) {
		return PyLong_AsLong(p);
	} else if (PyUnicode_Check(p)) {
		auto size = PyUnicode_GET_LENGTH(p);
		return std::string(PyUnicode_AsUTF8(p), size);
	} else if (PyTuple_Check(p)) {
		auto size = PyTuple_Size(p);
		std::vector<Flow::FlowVar> fTup(size);
		for (auto i = 0; i < size; i++) {
			//Circular references will break this boy
			fTup[i] = Py2Flow(PyTuple_GetItem(p, i));
		}
		return fTup;
	}
	return std::monostate();
}

PyObject* CatchPy(PyObject* p) {
	PyObject* err;
	if (p == nullptr && (err = PyErr_Occurred()) != nullptr) {
		PyObject* type, * value, * traceback;
		PyErr_Fetch(&type, &value, &traceback);
		auto pstr = PyObject_Str(value);
		throw std::runtime_error("Python Error: " + std::string(PyUnicode_AsUTF8(pstr)));
	}
	return p;
}

struct Flow::PythonBlockIMPL {
	static size_t count;
	size_t params;
	PyRef pModule;
	struct PyModuleDef def = {
		PyModuleDef_HEAD_INIT, 
		nullptr, 
		NULL,
		-1, 
		{}
	};
	PyRef pFunc = nullptr;
	PythonBlockIMPL() {
		count++;
		updatePythonVMState();
	}
	void compile(std::string const &source, std::string const& name) {
		PyRef byteCode(CatchPy(Py_CompileString(source.c_str(), name.c_str(), Py_file_input)));
		def.m_name = name.c_str();
		pModule = PyModule_Create(&def);
		PyEval_EvalCode(byteCode, PyModule_GetDict(pModule), nullptr);
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

Flow::FlowVar Flow::PythonBlock::execute(FlowVar args) {
	if (impl->pFunc == nullptr) throw "This Pythonblock is not ready yet!";
	if (!std::holds_alternative<Array>(args)) {
		Array newArgs;
		if (!std::holds_alternative<Flow::Empty>(args)) { //We want an empty tuple if it is delibritley empty
			newArgs.push_back(args);
		}
		args = FlowVar(newArgs);
	}
	auto pyArg = Flow2Py(args);
	auto pyResult = PyRef(CatchPy(PyObject_Call(impl->pFunc, pyArg, NULL))); //Call function in impl
	auto result = Py2Flow(pyResult); 
	return result;
}

void Flow::PythonBlock::precompile(){
	impl->compile(source,block()->name);
}

Flow::PythonBlock::~PythonBlock() {}