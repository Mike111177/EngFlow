#pragma once
#include <vector>

#include <ui/UITypes.h>
#include <ui/UIElement.h>
#include <ui/primitives/GradientRectangle.h>

namespace EngFlow {
	namespace UI {
		class Canvas : public UIElement, protected Primitives::GradientRectangle {
			std::vector<UIPTR> children;
			bool drawBackground;
		public:
			Canvas();
			virtual void draw() override;
			void addChild(UIPTR child);
			virtual void handleEvent(Event const&) override;
			void setDrawbackground(bool);
		};
	};
};