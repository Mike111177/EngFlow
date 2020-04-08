#include <ui/UITypes.h>
namespace EngFlow {
	namespace UI {
		UIRect::UIRect(UIPos p, UISize s): UIPos(p), UISize(s){}
		UIRect::UIRect(UIPos p, int w, int h): UIPos(p), UISize{ w, h }{}
		UIRect::UIRect(int x, int y, UISize s): UIPos{ x, y }, UISize(s) {}
		UIRect::UIRect(int x, int y, int w, int h): UIPos{ x, y }, UISize{ w, h } {}
		UIRect& EngFlow::UI::UIRect::operator=(UISize const& s) {
			w = s.w; h = s.h;
			return *this;
		}
		UIRect& UIRect::operator=(UIPos const& p){
			x = p.x; y = p.y;
			return *this;
		}
		bool is_inside(UIPos const& p, UIRect const& r){
			return r.x < p.x       &&
				   p.x < r.x + r.w &&
				   r.y < p.y       &&
				   p.y < r.y + r.h;
		}
		std::ostream& operator<<(std::ostream &out, UISize const& s){
			out << "UISize(" << s.w << "," << s.h << ")";
			return out;
		}
		std::ostream& operator<<(std::ostream &out, UIPos const& s){
			out << "UIPos(" << s.x << "," << s.y << ")";
			return out;
		}
		std::ostream& operator<<(std::ostream &out, UIRect const& s){
			out << "UIRect(" << s.x << "," << s.y << "," << s.w << "," << s.h << ")";
			return out;
		}
		UIPos UIVec::operator+(UIPos const& p){
			return { dx + p.x, dy + p.y };
		}
		UIPos& UIPos::operator-=(UIVec const& v){
			x -= v.dx;
			y -= v.dy;
			return *this;
		}
		UIPos UIPos::operator-(UIVec const& v){
			return { x - v.dx, y - v.dy };
		}
		UIPos& UIPos::operator+=(UIVec const& v){
			x += v.dx;
			y += v.dy;
			return *this;
		}
		UIPos UIPos::operator+(UIVec const& v){
			return { x + v.dx, y + v.dy };
		}
		UIVec UIPos::operator-(UIPos const& p){
			return { x - p.x, y - p.y };
		}
	}
}