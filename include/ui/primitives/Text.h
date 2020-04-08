#pragma once
#include <ui/UITypes.h>

namespace EngFlow {
	namespace UI {
		namespace Primitives {
			class Text {
				unsigned int vao = 0;
				unsigned int vbo = 0;
				bool buffersInitialized = false;
				void initBuffers();
				float scale = 1.0f;
				int fontSize = 30;
			public:
				Text(std::string, UIColor);
				std::string text;
				UIColor color;
				std::string font = "times";
				void draw(UIRect);
				UISize textSize() const;
			};
		}
	}
}