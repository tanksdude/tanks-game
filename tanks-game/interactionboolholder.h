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
