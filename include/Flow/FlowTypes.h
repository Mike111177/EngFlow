#pragma once
#include <iostream>
#include <variant>
namespace Flow{
	using FlowVar = std::variant<
		std::monostate,
		int,
		long
//		float,
//		double,
//		std::string
	>;
	struct PrintFlow {
		void operator()(std::monostate) {}
		void operator()(long i) { std::cout << i << std::endl; }
	};
	inline void print(FlowVar v) {
		std::visit(PrintFlow(), v);
	}
};