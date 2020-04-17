#pragma once
#include <string>
#include <iostream>
#include <variant>
#include <vector>
#include <optional>
#include <type_traits>
#include <typeinfo> 
#include <unordered_map>

namespace Flow{
	//Global Softtype
	template <typename T>
	concept FlowVarType_c = 
		std::constructible_from<FlowVar_v, T> && 
		!std::same_as<FlowVar_v, T> && 
		!std::same_as<FlowVar, T>;

	//stdlib rips
	class FlowVar;
	using Empty = std::monostate;
	using String = std::string;

	//Null Variables Sometimes do and sometimes dont have special meaning depending on the language
	struct Null {
		constexpr bool operator==(Null) noexcept { return true; }
		constexpr bool operator!=(Null) noexcept { return false; }
		constexpr bool operator<(Null) noexcept { return false; }
		constexpr bool operator>(Null) noexcept { return false; }
		constexpr bool operator<=(Null) noexcept { return true; }
		constexpr bool operator>=(Null) noexcept { return true; }
		constexpr operator char() noexcept { return 0; }
		constexpr operator int() noexcept { return 0; }
		constexpr operator long() noexcept { return 0; }
		constexpr operator unsigned char() noexcept { return 0; }
		constexpr operator unsigned int() noexcept { return 0; }
		constexpr operator unsigned long() noexcept { return 0; }
		constexpr operator void*() noexcept { return nullptr; }
	};

	struct Array : std::vector<FlowVar> {
		enum class Hint {TUPLE,	ARRAY} hint = Hint::TUPLE;
		using std::vector<FlowVar>::vector;
		template<FlowVarType_c T>
		Array(std::vector<T> const& vec) {
			reserve(vec.size());
			for (auto const& o : vec) emplace_back(o); 
		}
	};
	struct Dict : std::unordered_map<String, FlowVar> {

	};
	//There is probably a more elegant way to organize this
	using FlowVar_v = std::variant<Empty,Null,
		                           //bool, //bool breaks stuff, will find out later
								   signed char, signed short, signed int, signed long,
								   unsigned char, unsigned short, unsigned int, unsigned long,
								   float, double, //I might make all numerics their own subvarient...
								   String, Array, Dict>;
	//TODO add arbitrary precision type, matrix type

	//Concepts to control FlowVar operator overloading
	template <typename A, typename B>
	concept FlowVarCastable_c = requires (B b) { static_cast<A>(b); };

	class FlowVar : public FlowVar_v {
		template<FlowVarType_c A>
		struct castVisitor {
			template <FlowVarType_c B>
			std::optional<A> operator ()(B b) requires FlowVarCastable_c<A,B> {return static_cast<A>(b); }
			template <FlowVarType_c B>
			std::optional<A> operator ()(B b) requires !FlowVarCastable_c<A, B> {return std::nullopt;}
		};
	public:
		using FlowVar_v::FlowVar_v;
		template <FlowVarType_c T>
		std::optional<FlowVar> cast() {
			return std::visit(castVisitor<T>(), *this);
		}
		template <FlowVarType_c T>
		std::optional<T> castGet() {
			auto casted = cast<T>();
			if (casted) {
				return std::get<T>(casted.value());
			} else {
				return std::nullopt;
			}
		}
		FlowVar& operator[](size_t const&);
		FlowVar& operator[](std::string const&);
	};

	//File Management
	struct FlowResource {
		std::string name;
		std::vector<char> data;
		FlowResource(std::string const&, std::vector<char> const&);
	};
	using FlowResourceList = std::vector<FlowResource>;

	//Operators
	bool operator==(FlowVar const& a, FlowVar const& b);
	bool operator!=(FlowVar const& a, FlowVar const& b);
	std::ostream& operator<<(std::ostream&, FlowVar const&);

	bool operator==(Array const& a, Array const& b);
	std::ostream& operator<<(std::ostream&, Array const&);
};

namespace std {
	std::string to_string(Flow::FlowVar const&);
	std::string to_string(Flow::Array const&);
	std::string to_string(Flow::Dict const&);
}




