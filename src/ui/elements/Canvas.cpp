#include <ui/elements/Canvas.h>

namespace EngFlow {
	namespace UI {
		Canvas::Canvas(): Primitives::GradientRectangle({ 0,0,0,0 }), drawBackground(false) {
			color1 = { .8,.8,.8 };
			color2 = { .4,.4,.4 };
			vert = 1;
		}
		void Canvas::draw(){
			if (visible) {
				if (drawBackground) {
					Primitives::GradientRectangle::draw(getAllocation());
				}
				for (auto& child : children) {
					if (child->shouldFill()) {
						child->setAllocation(getAllocation());
					}
					child->draw();
				}
			}
		}
		void Canvas::addChild(UIPTR child){
			children.push_back(child);
		}
		void Canvas::handleEvent(Event const& event){
			for (auto& child : children) {
				child->handleEvent(event);
			}
		}
		void Canvas::setDrawbackground(bool enable) {
			drawBackground = enable;
		}
	};
};