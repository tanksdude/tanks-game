#pragma once

struct InteractionBoolHolder {
	bool shouldDie;
	bool otherShouldDie;
	InteractionBoolHolder() : InteractionBoolHolder(false, false) {}
	InteractionBoolHolder(bool a, bool b) {
		shouldDie = a;
		otherShouldDie = b;
	}
	InteractionBoolHolder(bool a) {
		shouldDie = a;
		otherShouldDie = false;
	}
};
