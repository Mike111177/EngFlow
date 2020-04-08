#pragma once
#include <ui/UITypes.h>

namespace EngFlow {
	namespace UI {
		namespace Primitives {
			class SolidRectangle: public UIRect {
			public:
				using UIRect::UIRect;
				UIColor color;
				void draw();
			};
		}
	}
}