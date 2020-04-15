#pragma once
#include <memory>
#include <optional>
#include <vector>
#include <string>
#include <unordered_map>

#include <Flow/FlowTypes.h>

namespace Flow {
	class Block final {
	public:
		std::string name;
		template<class T>
		static std::shared_ptr<Block> create() {
			std::shared_ptr<Block> ptr(new Block);
			*ptr = Block(T::LogicType, std::unique_ptr<Logic>(new T(ptr)));
			return ptr;
		}
		static std::optional<std::shared_ptr<Block>> create(std::string type);
		class Logic {
			const std::weak_ptr<Block> _block;
		public:
			Logic(std::weak_ptr<Block>);
			std::shared_ptr<Block> block();
			virtual void saveResources(std::unique_ptr<FlowResourceList> &);
			virtual void loadResources(FlowResourceList &);
			virtual ~Logic();
		};
		std::unique_ptr<Logic> const& logic();
		static std::unordered_map<std::string, decltype(&Flow::Block::create<Flow::Block::Logic>)> Registry;
		std::string const& getType() const;
	private:
		std::string type;
		std::unique_ptr<Logic> b_logic;
		
		Block(std::string type, std::unique_ptr<Logic>& logic);
		Block();
		//std::vector<std::shared_ptr<Port>> ports;
	};
}