#include <ui/App.h>
#include <Flow/FlowDocument.h>

int main() {
	Flow::FlowDocument doc;
	doc.create();
	doc.save("test.zip");
	EngFlow::UI::LaunchApp();
	doc.close();
	return 0;
}