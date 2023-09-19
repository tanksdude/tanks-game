#pragma once
#include <memory>

struct InteractionBoolHolder {
	bool shouldDie;
	bool otherShouldDie;
	InteractionBoolHolder(bool a, bool b) {
		shouldDie = a;
		otherShouldDie = b;
	}
	InteractionBoolHolder(bool a) : InteractionBoolHolder(a, false) {}
	InteractionBoolHolder() : InteractionBoolHolder(false, false) {}
};
//could use std::pair

template<typename T, typename U>
struct InteractionUpdateHolder {
	InteractionBoolHolder deaths;
	std::shared_ptr<T> firstUpdate;
	std::shared_ptr<U> secondUpdate;

	InteractionUpdateHolder(bool a, bool b, std::shared_ptr<T>& update1, std::shared_ptr<U>& update2) {
		deaths = InteractionBoolHolder(a, b);
		firstUpdate = update1;
		secondUpdate = update2;
	}
	InteractionUpdateHolder(bool a, bool b, T* update1, U* update2) {
		deaths = InteractionBoolHolder(a, b);
		firstUpdate = std::shared_ptr<T>(update1);
		secondUpdate = std::shared_ptr<U>(update2);
	}
	InteractionUpdateHolder(bool a, T* update1, U* update2) : InteractionUpdateHolder(a, false, update1, update2) {}
	~InteractionUpdateHolder() {
		//no delete because shared_ptr
	}
};
