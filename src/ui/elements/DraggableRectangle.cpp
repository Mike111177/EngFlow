#include <ui/elements/DraggableRectangle.h>

namespace EngFlow {
	namespace UI {
        DraggableRectangle::DraggableRectangle(int x, int y, int w, int h) : rect{ x, y, w, h } {
            rect.color = Colors::GREEN;
        }
        void DraggableRectangle::draw(){
            rect.draw();
        }
        void DraggableRectangle::handleEvent(Event const& event){
            if (std::holds_alternative<Events::MOUSE_MOVE>(event)) {
                auto mPos = std::get<Events::MOUSE_MOVE>(event);
                highlight = is_inside(mPos, rect);
                if (dragging) (UIRect&)rect = (mPos - mousePosStart) + rectPosStart;
                else mousePosStart = mPos;         
            }
            if (std::holds_alternative<Events::MOUSE_PRESS>(event) && highlight) {
                rectPosStart = rect;
                dragging = true;
            }
            if (std::holds_alternative<Events::MOUSE_RELEASE>(event)) {
                dragging = false;
            }
            if (highlight || dragging) {
                rect.color = Colors::RED;
            }
            else {
                rect.color = Colors::GREEN;
            }
        }


	};
};