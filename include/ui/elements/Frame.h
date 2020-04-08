#pragma once
#include <ui/Window.h>
#include <ui/UIElement.h>

#include "Canvas.h"
namespace EngFlow {
	namespace UI {
		class Frame : public Canvas {
			std::shared_ptr<Window> window;
		public:
			using Canvas::addChild;

			Frame(std::shared_ptr<Window>);
			std::shared_ptr<Window> getWindow();
			virtual void handleEvent(Event const&) override;
			virtual void draw() override;
		};
	};
};