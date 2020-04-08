#include <ui/elements/Label.h>

EngFlow::UI::Label::Label() : text("Hello", Colors::BLACK) {}
EngFlow::UI::Label::Label(std::string labeltext): text(labeltext, Colors::BLACK) {}
EngFlow::UI::Label::Label(std::string labeltext, std::string font): text(labeltext, Colors::BLACK) {
	text.font = font;
}

void EngFlow::UI::Label::draw(){
	text.draw(getAllocation());
}

EngFlow::UI::UISize EngFlow::UI::Label::minimum_size() const {
	return text.textSize();
}
