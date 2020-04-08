#pragma once
#include <ui/elements/Frame.h>

#include "Window.h"

namespace EngFlow {
	namespace UI {
		struct RendererData;
		class Renderer {
			RendererData* data;
			std::shared_ptr<Frame> frame;
		public:
			Renderer(std::shared_ptr<Frame>);
			~Renderer();
			void init();
			void render();
		};
	};
};