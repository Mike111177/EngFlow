#include <Flow/Block.h>

std::unordered_map<std::string, decltype(&Flow::Block::create<Flow::Block::Logic>)> Flow::Block::Registry;

Flow::Block::Logic::Logic(std::weak_ptr<Block> b): _block(b) {}

std::shared_ptr<Flow::Block> Flow::Block::Logic::block() {
	return _block.lock();
}

void Flow::Block::Logic::saveResources(std::unique_ptr<FlowResourceList> &) {}


void Flow::Block::Logic::loadResources(FlowResourceList&) {}

Flow::Block::Logic::~Logic() {}

std::optional<std::shared_ptr<Flow::Block>> Flow::Block::create(std::string type) {
	auto it = Registry.find(type);
	if (it == Registry.end()) {
		return std::nullopt;
	} else {
		return (*it).second();
	}
}

std::unique_ptr<Flow::Block::Logic> const& Flow::Block::logic() {
	return b_logic;
}

std::string const& Flow::Block::getType() const {
	return type;
}

Flow::Block::Block(std::string type, std::unique_ptr<Logic>& logic): type(type), b_logic(std::move(logic)) {}

Flow::Block::Block() {}