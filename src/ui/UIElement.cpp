#include <ui/UIElement.h>

namespace EngFlow {
	namespace UI {
		UIRect UIElement::getAllocation() const {
			return _allocation;
		}
		void UIElement::setAllocation(UIRect r) {
			_allocation = r;
		}
		void UIElement::draw(){}

		void UIElement::handleEvent(Event const&) {}
		bool UIElement::shouldFill() const {
			return true;
		}
		UISize UIElement::minimum_size() const {
			return { 0,0 };
		}
	};
};