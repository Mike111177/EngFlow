#include <Flow/Block.h>

std::unordered_map<std::string, decltype(&Flow::Block::constructBlock<Flow::Block::Logic>)> Flow::Block::Registry;

Flow::Block::Logic::Logic(std::weak_ptr<Block> b): _block(b) {}

Flow::Block& Flow::Block::Logic::block() {
	return *_block.lock();
}

Flow::FlowResourceList& Flow::Block::Logic::saveResources(FlowResourceList& l) { return l; }

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

Flow::Block::Logic* Flow::Block::logic() {
	return b_logic.get();
}

Flow::Block::Logic* const Flow::Block::logic() const {
	return b_logic.get();
}

std::string const& Flow::Block::type() const{
	return _type;
}

Flow::Block::Block(std::string const& type, std::unique_ptr<Logic>& logic): _type(type), b_logic(std::move(logic)) {}

Flow::Block::Block() {}