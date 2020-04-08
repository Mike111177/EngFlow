#include <algorithm>

#include <ui/layout/Stack.h>

void EngFlow::UI::Stack::draw() {
	auto a = getAllocation();
	if (vertical) {
		int y = 0;
		for (auto i = children.rbegin(); i != children.rend(); ++i) { //WeeWoo
			auto child = *i;
			auto ms = child->minimum_size();
			UIRect an{ a.x, y, ms };
			child->setAllocation(an);
			child->draw();
			y += ms.h;
		}
	} else {
		int x = 0;
		for (auto& child : children) {
			auto ms = child->minimum_size();
			UIRect an{ x, a.y, ms };
			child->setAllocation(an);
			child->draw();
			x += ms.w;
		}
	}
}

EngFlow::UI::UISize EngFlow::UI::Stack::minimum_size() const {
	int w = 0;
	int h = 0;
	for (auto& child : children) {
		auto ms = child->minimum_size();
		if (vertical) {
			h += ms.h;
			w = std::max(w, ms.w);
		}
		else {
			w += ms.w;
			h = std::max(h, ms.h);
		}
	}
	return { w, h };
}

void EngFlow::UI::Stack::addChild(UIPTR child) {
	children.push_back(child);
}
