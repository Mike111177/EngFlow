#pragma once
#include <ui/UITypes.h>

namespace EngFlow {
	namespace UI {
		namespace Primitives {
			class GradientRectangle: public UIRect {
			public:
				using UIRect::UIRect;
				UIColor color1;
				UIColor color2;
				unsigned int vert;
				void draw(UIRect);
			};
		}
	}
}