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



	//tracked_pointer
	//Like a shared ptr that can only have weak copies
	template <typename T>
	class utracked_ptr {
		struct controlblock {
			T* object = nullptr;
			//Count = 0 Does not prevent destruction of object, think like a weak_ptr 
			size_t count = 0; 
			bool empty : 1 = true; //object is nullptr or uninitialized
			bool destroyed : 1 = false; //utracked_ptr that created cb was destroyed
		} * cb;
		utracked_ptr(utracked_ptr<T>&) = delete;
	public:
		class tracker {
			//Essentially a weak reference
			controlblock* cb;
			tracker(controlblock* cb) : cb(cb) { cb->count++; }
			friend tracker utracked_ptr<T>::track();
		public:
			tracker(utracked_ptr<T> ptr) : cb(ptr.cb) { cb->count++; }
			tracker(tracker& t) : cb(t.cb) { cb->count++; }
			~tracker() {
				cb->count--;
				//If owner utracked_ptr destroyed, and no more trackers tracking it, destroy.
				if (cb->destroyed && cb->count == 0) delete cb;
			}
			tracker track() { return tracker(cb); }
			operator bool() { return !cb->destroyed && !cb->empty; }
			T* operator->() { return cb->object; }
			T* operator*() { return cb->object; }
			T* get() { return cb->object; }
		};
		utracked_ptr() : cb(new controlblock) {}
		utracked_ptr(T* ptr) : cb(new controlblock) {
			cb->object = ptr;
			cb->empty = false;
		}
		~utracked_ptr() {
			//Destroy owned object
			delete cb->object;
			if (cb->count == 0) { //If no trackers tracking it, destroy.
				delete cb;
			} else { //Else mark as empty and destroyed so trackers know to destory when done.
				cb->empty = true;
				cb->destroyed = true;
			}
		}
		tracker track() {return tracker(cb);}
		explicit operator bool() {return !cb->destroyed && !cb->empty;}
		T* operator->() { return cb->object; }
		T* operator*() { return cb->object; }
		T* get() { return cb->object; }
		void swap(utracked_ptr<T>& ptr) {
			auto t = ptr.get();
			ptr.cb->object = cb->object;
			cb->object = t;
		}
		

	};
	template<typename T, typename... Args>
	utracked_ptr<T> make_utracked(Args... args) {
		return new T(args...);
	}
}