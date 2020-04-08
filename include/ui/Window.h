#pragma once
#include "UITypes.h"
#include "Events.h"

namespace EngFlow {
	namespace UI {
		class WindowIMPL;
		class Window {
			WindowIMPL* impl;
		public:
			Window();
			~Window();
			bool open();
			void close();
			Event pollEvents();
			void glswap();
			UISize getSize();
		};
	};
};