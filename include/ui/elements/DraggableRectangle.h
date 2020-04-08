#pragma once
#include <ui/primitives/SolidRectangle.h>

#include <ui/UIElement.h>
namespace EngFlow {
	namespace UI {
		class DraggableRectangle: public UIElement {
			bool highlight = false;
			bool mouseDown = false;
			bool dragging = false;
			bool initialized = false;
			UIPos mousePosStart = { 0,0 };
			UIPos rectPosStart = { 0,0 };
			Primitives::SolidRectangle rect = { 20,20, 150,100 };
		public:
			DraggableRectangle(int x, int y, int w, int h);
			virtual void draw() override;
			virtual void handleEvent(Event const&) override;
			//	virtual void render() = 0;
		};
	};
};