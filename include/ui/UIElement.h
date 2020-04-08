#pragma once
#include <memory>
#include <optional>

#include <ui/Events.h>

#include <ui/UITypes.h>

namespace EngFlow {
	namespace UI {
		class UIElement {
		private:
			UIRect _allocation = { 0,0,0,0 };
		public:
			bool visible = false;
			virtual ~UIElement() {}
			UIRect getAllocation() const;
			void setAllocation(UIRect);
			virtual void draw();
			virtual void handleEvent(Event const&);

			//Layout Management
			//Does this element want to fill as much space as possible
			virtual bool shouldFill() const;
			//What is the minimum size this element would like to be
			virtual UISize minimum_size() const;

		};
		using UIPTR = std::shared_ptr<UIElement>;
	};
};