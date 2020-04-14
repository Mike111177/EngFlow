#pragma once
#include <string>
#include <iostream>
#include <variant>
#include <vector>

namespace Flow{
	using FlowVar = std::variant<
		std::monostate,
		int,
		long,
		std::string
		//,std::vector<FlowVar> //Tuple? This is technically recursive but i think this isnt impossible to acheive... Variants are cool...
	>;

	struct FlowResource {
		std::string name;
		std::vector<char> data;
	};

	using FlowResourceList = std::vector<FlowResource>;
};

namespace std {
	std::string to_string(Flow::FlowVar&);
}
std::ostream& operator<<(std::ostream &, Flow::FlowVar&);