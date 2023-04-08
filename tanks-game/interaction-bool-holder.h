#pragma once

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
	T firstUpdate;
	U secondUpdate;

	InteractionUpdateHolder(bool a, bool b, T update1, U update2) {
		deaths = InteractionBoolHolder(a, b);
		firstUpdate = update1;
		secondUpdate = update2;
	}
};
