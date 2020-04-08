#pragma once
#include <ostream>
namespace EngFlow {
	namespace UI {
		struct UISize;
		struct UIPos;
		struct UIVec;
		struct UIRect;

		struct UISize {
			int w, h;
		};
		struct UIVec {
			int dx, dy;
			UIPos operator+(UIPos const&);
		};
		struct UIPos {
			int x, y;

			UIPos& operator-=(UIVec const&);
			UIPos operator-(UIVec const&);
			UIPos& operator+=(UIVec const&);
			UIPos operator+(UIVec const&);

			UIVec operator-(UIPos const&);
		};
		struct UIRect: public UIPos, public UISize {
			UIRect(UIPos, UISize);
			UIRect(UIPos, int, int);
			UIRect(int, int, UISize);
			UIRect(int, int, int, int);
			UIRect& operator=(UISize const&);
			UIRect& operator=(UIPos const&);
		};

		bool is_inside(UIPos const&, UIRect const&);

		std::ostream& operator<<(std::ostream &out, UISize const&);
		std::ostream& operator<<(std::ostream &out, UIPos const&);
		std::ostream& operator<<(std::ostream &out, UIRect const&);

		struct UIColor {
			float r, g, b;
		};
		namespace Colors {
			const UIColor RED{ 1,0,0 };
			const UIColor GREEN{ 0,1,0 };
			const UIColor BLUE{ 0,0,1 };
			const UIColor BLACK{ 0,0,0 };
		}
	}
}
