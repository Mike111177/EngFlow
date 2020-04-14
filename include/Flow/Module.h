#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <memory>
#include <utility>
#include <unordered_map>
#include <functional>

#include <Flow/FlowDocument.h>

namespace Flow {
	class Block;
	struct Link;
	struct Port {
		std::shared_ptr<Link> link;
		bool isConnected();
	};
	struct Link {
		std::vector<std::weak_ptr<Port>> ports;
	};
	class Module {
		std::vector<std::weak_ptr<Link>> links;
		std::vector<std::unique_ptr<Block>> blocks;
		void connectPorts(std::shared_ptr<Port>& a, std::shared_ptr<Port>& b);
	};
}