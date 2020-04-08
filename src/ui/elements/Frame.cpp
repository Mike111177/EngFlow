#include <iostream>
#include <GL/glew.h>

#include <ui/elements/Frame.h>

namespace EngFlow {
	namespace UI {
		Frame::Frame(std::shared_ptr<Window> window) : window(window) {
			//setDrawbackground(true);
			UISize s = window->getSize();
			w = s.w; h = s.h;
		}
		std::shared_ptr<Window> Frame::getWindow(){return window;}
		void Frame::handleEvent(Event const& event){
			if (std::holds_alternative<Events::WINDOW_RESIZE>(event)) {
				UISize s = std::get<Events::WINDOW_RESIZE>(event);
				setAllocation({ 0,0,s });
				return;
			}
			if (std::holds_alternative<Events::WINDOW_EXPOSED>(event)) {
				UISize s = window->getSize();
				setAllocation({ 0,0,s });
				visible = true;
				return;
			}
			if (std::holds_alternative<Events::WINDOW_SHOWN>(event)) {
				visible = true;
				return;
			}
			if (std::holds_alternative<Events::WINDOW_HIDDEN>(event)) {
				visible = false;
				return;
			}
			Canvas::handleEvent(event);
		}
		void Frame::draw(){
			UISize s = window->getSize();
			setAllocation({ 0,0,s });
			Canvas::draw();
		}
	};
};