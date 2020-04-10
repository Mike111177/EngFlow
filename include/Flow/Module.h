#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <memory>

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
		template<class T, class... Args>
		static std::shared_ptr<Block> create(Args... args) {
			std::shared_ptr<Block> ptr(new Block);
			*ptr = Block(std::unique_ptr<Logic>(new T(ptr, args...)));
			return ptr;
		}
		class Logic {
			const std::weak_ptr<Block> _block;
		public:
			Logic(std::weak_ptr<Block>);
			std::shared_ptr<Block> block();
			virtual ~Logic();
		};
		std::unique_ptr<Logic> const& logic();
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