#include <ui/elements/Canvas.h>
#include <ui/elements/Label.h>
#include <ui/layout/Stack.h>


namespace EngFlow {
	namespace UI {
		class MainAppMenu : public UIElement {
			Stack s;
			Canvas can;
		public:
			MainAppMenu();
			virtual void draw() override;
			virtual void handleEvent(Event const&) override;
		};
	}
}