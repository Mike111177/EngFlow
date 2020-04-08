#include <iostream>

#include <ui\elements\DraggableRectangle.h>
#include <ui\menus\MainAppMenu.h>

#define TOOLBAR_HEIGHT 15 

EngFlow::UI::MainAppMenu::MainAppMenu() {
	UIPTR l1(new Label("Hello", "times"));
	UIPTR l2(new Label("World", "arial"));
	UIPTR l3(new Label("Text", "ALGER"));
	UIPTR l4(new Label("OpenGL", "calibri"));
	UIPTR rect(new DraggableRectangle({10,10,40,40}));
	s.addChild(l1);
	s.addChild(l2);
	s.addChild(l3);
	s.addChild(l4);
	can.setDrawbackground(true);
	can.visible = true;
	can.addChild(rect);
}

void EngFlow::UI::MainAppMenu::draw() {
	auto all = getAllocation();
	auto sll = s.minimum_size();
	UIRect fll{ 0,all.h - sll.h, sll };
	s.setAllocation(fll);
	s.draw();
	UIRect cll{ 0,0,all.w,all.h - fll.h };
	can.setAllocation(cll);
	can.draw();
}

void EngFlow::UI::MainAppMenu::handleEvent(Event const& event) {
	can.handleEvent(event);
}
