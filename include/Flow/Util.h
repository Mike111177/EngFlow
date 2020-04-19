#pragma once
#include <memory>
namespace Flow{
	//Use this to create a static member with reference counting
	//and auto destruction. Must be Default Constructable
	//Change num parameter to specify multiple members of the same Owner and type T
	template <typename Owner, typename T, size_t num = 0>
	class static_ptr {
		static size_t count;
		static std::unique_ptr<T> member;
		static void update() {
			if (count == 0 && member) {
				member.reset();
			} else if (count > 0 && !member) {
				member.reset();
				member = std::move(std::make_unique<T>());
			}
		}
	public:
		auto operator->() {return member.operator->();}
		auto operator*() {return member.operator*();}
		auto get() {return member.get();}
		operator T* () { return get(); }
		static_ptr() {
			count++;
			update();
		}
		~static_ptr() {
			count--;
			update();
		}
	};
	template <typename Owner, typename T, size_t num>
	size_t static_ptr<Owner, T, num>::count = 0;
	template <typename Owner, typename T, size_t num>
	std::unique_ptr<T> static_ptr<Owner, T, num>::member;
}