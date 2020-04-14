#include <Flow/FlowTypes.h>

static struct {
	std::string operator()(std::string& var) { return var; }
	std::string operator()(std::monostate) { return "NullVar"; }
	template <typename T>
	std::string operator()(T var) { return std::to_string(var); }
} flow2str;

std::string std::to_string(Flow::FlowVar &v) {
	return std::visit(flow2str, v);
}

std::ostream& operator<<(std::ostream& o, Flow::FlowVar& v) {
	return o << std::to_string(v);
}
