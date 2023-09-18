#include "priority-handler.h"

#include <iostream>

PriorityResult PriorityHandler::determinePriority(const Bullet* a, const Bullet* b) {
	double a_offense = a->getOffenseTier();
	double a_defense = a->getDefenseTier();
	double b_offense = b->getOffenseTier();
	double b_defense = b->getDefenseTier();

	bool a_dies = false, b_dies = false;

	if (a_offense >= b_defense) {
		b_dies = true;
	}
	if (b_offense >= a_defense) {
		a_dies = true;
	}

	if (a_dies && b_dies) {
		return PriorityResult::bothDie;
	}
	if (a_dies) {
		return PriorityResult::firstDies;
	}
	if (b_dies) {
		return PriorityResult::secondDies;
	}
	return PriorityResult::neitherDie;
}

PriorityResult PriorityHandler::determinePriority(const Tank* a, const Bullet* b) {
	double a_offense = a->getOffenseTier();
	double a_defense = a->getDefenseTier();
	double b_offense = b->getOffenseTier();
	double b_defense = b->getDefenseTier();

	bool a_dies = false, b_dies = false;

	if (a_offense >= b_defense || a_defense >= b_offense) {
		b_dies = true;
	}
	if (b_offense >= a_defense) {
		a_dies = true;
	}

	if (a_dies && b_dies) {
		return PriorityResult::bothDie;
	}
	if (a_dies) {
		return PriorityResult::firstDies;
	}
	if (b_dies) {
		return PriorityResult::secondDies;
	}
	return PriorityResult::neitherDie;
}

PriorityResult PriorityHandler::determinePriority(const Tank* a, const Tank* b) {
	double a_offense = a->getOffenseTier();
	double a_defense = a->getDefenseTier();
	double b_offense = b->getOffenseTier();
	double b_defense = b->getDefenseTier();

	bool a_dies = false, b_dies = false;

	if (a_offense > b_defense) {
		b_dies = true;
	}
	if (b_offense > a_defense) {
		a_dies = true;
	}

	if (a_dies && b_dies) {
		return PriorityResult::bothDie;
	}
	if (a_dies) {
		return PriorityResult::firstDies;
	}
	if (b_dies) {
		return PriorityResult::secondDies;
	}
	return PriorityResult::neitherDie;
}

PriorityResult PriorityHandler::determinePriority(const Bullet* a, const CircleHazard* b) {
	double a_offense = a->getOffenseTier();
	double a_defense = a->getDefenseTier();
	double b_offense = b->getOffenseTier();
	double b_defense = b->getDefenseTier();

	bool a_dies = false, b_dies = false;

	if (a_offense >= b_defense) {
		b_dies = true;
	}
	switch (b->getCollisionType()) {
		default:
			std::cerr << "WARNING: unknown CircleHazardCollisionType!" << std::endl;
		case CircleHazardCollisionType::solid:
			if (b_offense >= a_defense || b_defense > a_offense) {
				a_dies = true;
			}
			break;
		case CircleHazardCollisionType::under:
			if (b_offense >= a_defense) {
				a_dies = true;
			}
			break;
	}

	if (a_dies && b_dies) {
		return PriorityResult::bothDie;
	}
	if (a_dies) {
		return PriorityResult::firstDies;
	}
	if (b_dies) {
		return PriorityResult::secondDies;
	}
	return PriorityResult::neitherDie;
}

PriorityResult PriorityHandler::determinePriority(const Bullet* a, const RectHazard* b) {
	double a_offense = a->getOffenseTier();
	double a_defense = a->getDefenseTier();
	double b_offense = b->getOffenseTier();
	double b_defense = b->getDefenseTier();

	bool a_dies = false, b_dies = false;

	if (a_offense >= b_defense) {
		b_dies = true;
	}
	switch (b->getCollisionType()) {
		default:
			std::cerr << "WARNING: unknown RectHazardCollisionType!" << std::endl;
		case RectHazardCollisionType::solid:
			if (b_offense >= a_defense || b_defense > a_offense) {
				a_dies = true;
			}
			break;
		case RectHazardCollisionType::under:
			if (b_offense >= a_defense) {
				a_dies = true;
			}
			break;
	}

	if (a_dies && b_dies) {
		return PriorityResult::bothDie;
	}
	if (a_dies) {
		return PriorityResult::firstDies;
	}
	if (b_dies) {
		return PriorityResult::secondDies;
	}
	return PriorityResult::neitherDie;
}

PriorityResult PriorityHandler::determinePriority(const Tank* a, const CircleHazard* b) {
	double a_offense = a->getOffenseTier();
	double a_defense = a->getDefenseTier();
	double b_offense = b->getOffenseTier();
	double b_defense = b->getDefenseTier();

	bool a_dies = false, b_dies = false;

	if (a_offense > b_defense) {
		b_dies = true;
	}
	if (b_offense > a_defense) {
		a_dies = true;
	}

	if (a_dies && b_dies) {
		return PriorityResult::bothDie;
	}
	if (a_dies) {
		return PriorityResult::firstDies;
	}
	if (b_dies) {
		return PriorityResult::secondDies;
	}
	return PriorityResult::neitherDie;
}

PriorityResult PriorityHandler::determinePriority(const Tank* a, const RectHazard* b) {
	double a_offense = a->getOffenseTier();
	double a_defense = a->getDefenseTier();
	double b_offense = b->getOffenseTier();
	double b_defense = b->getDefenseTier();

	bool a_dies = false, b_dies = false;

	if (a_offense > b_defense) {
		b_dies = true;
	}
	if (b_offense > a_defense) {
		a_dies = true;
	}

	if (a_dies && b_dies) {
		return PriorityResult::bothDie;
	}
	if (a_dies) {
		return PriorityResult::firstDies;
	}
	if (b_dies) {
		return PriorityResult::secondDies;
	}
	return PriorityResult::neitherDie;
}
