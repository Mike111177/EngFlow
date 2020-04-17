#include <sstream>
#include <concepts>
#include <typeinfo>
#include <stdexcept>

#include <Flow/FlowTypes.h>

static struct {
	std::string operator()(std::string var) { return var; }
	std::string operator()(Flow::Null) { return "<Null>"; }
	std::string operator()(Flow::Empty) { return "<EMPTY>"; }
	template <typename T>
	std::string operator()(T var) { return std::to_string(var); }
} flow2str;

static struct {
	template<typename A, typename B>
	bool operator()(A a, B b) requires (std::equality_comparable_with<A, B>) {
		return a == b; 
	}
	template<typename A, typename B>
	bool operator()(A, B) requires (!std::equality_comparable_with<A, B>) {return false;}
} equalV;

bool Flow::operator==(FlowVar const&a, FlowVar const&b) {
	return std::visit(equalV, a, b);
}

std::string std::to_string(Flow::FlowVar const&v) {
	return std::visit(flow2str, v);
}

std::string std::to_string(Flow::Array const&arr) {
	std::ostringstream ss;
	std::string wrap;
	switch (arr.hint) {
		using enum Flow::Array::Hint;
	case TUPLE:
		wrap = "()";
		break;
	case ARRAY:
		wrap = "[]";
		break;
	default:
		wrap = "  ";
	}
	ss << wrap[0];
	bool first = true;
	//Dont you dare give this a circular reference, I aint handling it
	for (auto& item : arr) {
		if (!first) {
			ss << ", ";
		}
		ss << std::to_string(item);
		first = false;
	}
	ss << wrap[1];
	return ss.str();
}

std::string std::to_string(Flow::Dict const& arr) {
	std::ostringstream ss;
	std::string wrap;
	ss << "{";
	bool first = true;
	//Dont you dare give this a circular reference, I aint handling it
	for (auto& item : arr) {
		if (!first) {
			ss << ", ";
		}
		ss << std::to_string(item.first) << ": " << std::to_string(item.second);
		first = false;
	}
	ss << "}";
	return ss.str();
}

std::ostream& Flow::operator<<(std::ostream& o, FlowVar const&v) {
	return o << std::to_string(v);
}

std::ostream& Flow::operator<<(std::ostream& o, Array const&v) {
	return o << std::to_string(v);
}

bool Flow::operator==(Array const& a, Array const& b) {
	return (a.size()==b.size()) && std::equal(a.begin(), a.end(), b.begin());
}

bool Flow::operator!=(FlowVar const& a, FlowVar const& b) {
	return !(a==b);
}

Flow::FlowResource::FlowResource(std::string const& filename, std::vector<char> const& filedata): name(filename), data(filedata) {}

//Someone smarter than me can make this pretty. All is one in the eyes of the compiler.
template <typename T, typename K>
concept sqaureBrackAble = requires (T &t, K k) {
	{ t[k] }->std::convertible_to<Flow::FlowVar&>;
};

template<typename A, typename K>
Flow::FlowVar& operatorSqBrack(Flow::FlowVar& a, K i) {
	return std::get<A>(a)[i];
}

template <typename K>
struct opSqBrack {
	using RetType = decltype(&operatorSqBrack<std::vector<Flow::FlowVar>, K>);
	template<typename A>
	RetType operator()(A) requires (sqaureBrackAble<A, K>) {
		return operatorSqBrack<A>;
	}
	template<typename A>
	RetType operator()(A) requires (!sqaureBrackAble<A, K>) {
		throw std::runtime_error(std::string(typeid(A).name()) + " has no operator[" + std::string(typeid(K).name())+ "].");
	}
};

Flow::FlowVar& Flow::FlowVar::operator[](size_t const& s) {
	return std::visit(opSqBrack<size_t>(), *this)(*this, s);
}

Flow::FlowVar& Flow::FlowVar::operator[](std::string const& s) {
	return std::visit(opSqBrack<std::string>(), *this)(*this, s);
}
