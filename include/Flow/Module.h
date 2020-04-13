#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <memory>
#include <utility>
#include <unordered_map>
#include <functional>

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
	class Block final{
	public:
		template<class T>
		static std::shared_ptr<Block> create() {
			std::shared_ptr<Block> ptr(new Block);
			*ptr = Block(std::unique_ptr<Logic>(new T(ptr)));
			return ptr;
		}
		static std::optional<std::shared_ptr<Block>> create(std::string type);
		class Logic {
			const std::weak_ptr<Block> _block;
		public:
			Logic(std::weak_ptr<Block>);
			std::shared_ptr<Block> block();
			virtual ~Logic();
		};
		std::unique_ptr<Logic> const& logic();
		static std::unordered_map<std::string, decltype(&Flow::Block::create<Flow::Block::Logic>)> Registry;
	private:
		std::unique_ptr<Logic> _logic;
		Block(std::unique_ptr<Logic>& logic);
		Block();
		std::vector<std::shared_ptr<Port>> ports;
	};
	class Module {
		std::vector<std::weak_ptr<Link>> links;
		std::vector<std::unique_ptr<Block>> blocks;
		void connectPorts(std::shared_ptr<Port>& a, std::shared_ptr<Port>& b);
	};
}