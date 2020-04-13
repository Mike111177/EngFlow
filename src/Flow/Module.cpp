#include <Flow\Module.h>


std::unordered_map<std::string, decltype(&Flow::Block::create<Flow::Block::Logic>)> Flow::Block::Registry;

void Flow::Module::connectPorts(std::shared_ptr<Port>& a, std::shared_ptr<Port>& b) {

	if (!a->isConnected() && !b->isConnected()) {
		//Create
		auto newLink = std::make_shared<Link>();
		a->link = newLink;
		b->link = newLink;
		newLink->ports.push_back(a);
		newLink->ports.push_back(b);
		links.push_back(newLink);
	}
	else if (!a->isConnected()) {
		std::weak_ptr<Link> aLink = a->link;
		auto bLink = b->link;
		a->link = b->link;
		if (aLink.expired()) {
			//links.erase(*aLink);
		}
	}
	else if (!b->isConnected()) {
		std::weak_ptr<Link> bLink = b->link;
		auto aLink = a->link;
		b->link = a->link;
		if (bLink.expired()) {
			//links.erase(*bLink);
		}
	}
	else {
		//Merge
		auto aLink = a->link;
		auto bLink = b->link;
		for (auto &port : aLink->ports) {
			if (!port.expired())
				port.lock()->link = bLink;
		}
	}
}

bool Flow::Port::isConnected() {
	return link.use_count()>1;
}

Flow::Block::Logic::Logic(std::weak_ptr<Block> b): _block(b) {}

std::shared_ptr<Flow::Block> Flow::Block::Logic::block() {
	return _block.lock();
}

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
	return _logic;
}

Flow::Block::Block(std::unique_ptr<Logic>& logic): _logic(std::move(logic)) {}

Flow::Block::Block() {}
