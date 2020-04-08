#pragma once
#include <vector>

#include <ui/UIElement.h>

namespace EngFlow {
	namespace UI {
		class Stack : public UIElement {
			std::vector<UIPTR> children;
		public:
			bool vertical = false;
			virtual void draw() override;
			virtual UISize minimum_size() const override;
			void addChild(UIPTR child);
		};
	}
}