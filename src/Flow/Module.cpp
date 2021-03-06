#include <Flow\Module.h>

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
