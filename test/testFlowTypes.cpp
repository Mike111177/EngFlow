#include "doctest.h"

#include <Flow/FlowTypes.h>

TEST_CASE("FlowVars can be constructed"){
	using namespace Flow;
	CHECK_NOTHROW(FlowVar(char(0)));
	CHECK_NOTHROW(FlowVar(int(0)));
	CHECK_NOTHROW(FlowVar(long(0)));
	CHECK_NOTHROW(FlowVar(unsigned char(0)));
	CHECK_NOTHROW(FlowVar(unsigned int(0)));
	CHECK_NOTHROW(FlowVar(unsigned long(0)));
	CHECK_NOTHROW(FlowVar(signed char(0)));
	CHECK_NOTHROW(FlowVar(signed int(0)));
	CHECK_NOTHROW(FlowVar(signed long(0)));
	CHECK_NOTHROW(FlowVar(float(0)));
	CHECK_NOTHROW(FlowVar(double(0)));
	CHECK_NOTHROW(FlowVar(Empty{}));
	CHECK_NOTHROW(FlowVar(Null{}));
	CHECK_NOTHROW(FlowVar(Array{}));
	CHECK_NOTHROW(FlowVar(Dict{}));
}

TEST_CASE("FlowVars string work as expected") {
	using namespace Flow;
	FlowVar tVar;
	CHECK_NOTHROW(tVar = "Test String");
	CHECK(tVar == "Test String");
	CHECK(std::to_string(tVar) == "Test String");
	CHECK(std::get<String>(tVar) == "Test String");
}

TEST_CASE("FlowVars arrays work as expected") {
	using namespace Flow;
	FlowVar tVar;
	CHECK_NOTHROW(tVar = Array(3));
	CHECK_NOTHROW(tVar[0] = 1);
	CHECK_NOTHROW(tVar[1] = "banana");
	CHECK_NOTHROW(std::to_string(tVar));
	CHECK(tVar[0] == 1);
	CHECK(tVar[1] == "banana");
	CHECK(tVar[2] == FlowVar());
	CHECK(std::to_string(tVar) == std::to_string(std::get<Array>(tVar)));
	//CHECK_NOTHROW(for (FlowVar f: tVar)); //Implementing iterator lookthrough later
	CHECK_THROWS(tVar["banana"]); //It should not be accepting string indices

}

TEST_CASE("FlowVars dicts work as expected") {
	using namespace Flow;
	FlowVar tVar;
	CHECK_NOTHROW(tVar = Dict());
	CHECK_NOTHROW(tVar["banana"] = 1);
	CHECK_NOTHROW(tVar["apple"] = 5);
	CHECK_NOTHROW(std::to_string(tVar));
	CHECK(tVar["banana"] == 1);
	CHECK(tVar["apple"] == 5);
	CHECK(std::to_string(tVar) == std::to_string(std::get<Dict>(tVar)));
	//CHECK_NOTHROW(for (FlowVar f: tVar)); //Implementing iterator lookthrough later
	CHECK_THROWS(tVar[0]); //It should not be accepting numeric indices
}