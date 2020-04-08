#pragma once
#include <ui/UIElement.h>

#include <ui/primitives/Text.h>

namespace EngFlow {
	namespace UI {
		class Label: public UIElement {
			Primitives::Text text;
		public:
			Label();
			Label(std::string labeltext);
			Label(std::string labeltext, std::string font);
			virtual void draw() override;
			virtual UISize minimum_size() const override;
		};
	};
};