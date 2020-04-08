#pragma once
#include <variant>

#include <ui/UITypes.h>	

namespace EngFlow {
	namespace UI {
		namespace Events {
			using NO_EVENT = std::monostate;
			struct QUIT{};
			struct MOUSE_MOVE : public UIPos {};
			struct MOUSE_PRESS{};
			struct MOUSE_RELEASE{};
			struct WINDOW_RESIZE : public UISize{};
			struct WINDOW_EXPOSED {};
			struct WINDOW_SHOWN {};
			struct WINDOW_HIDDEN {};
			using Event = std::variant<
			    NO_EVENT,
				QUIT,
				MOUSE_MOVE,
				MOUSE_PRESS,
				MOUSE_RELEASE,
				WINDOW_RESIZE,
				WINDOW_EXPOSED,
				WINDOW_SHOWN,
				WINDOW_HIDDEN
				>;
		}
		using Event = Events::Event;
	}
}