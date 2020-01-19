#pragma once
#include "constants.h"
#include "bulletpriorityhandler.h"
#include "bullet.h"

char BulletPriorityHandler::determinePriority(Bullet* a, Bullet* b) {
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
		return -1;
	}
	if (a_dies) {
		return 0;
	}
	if (b_dies) {
		return 1;
	}
	return 2;
}

char BulletPriorityHandler::determinePriority(Bullet* a, Tank* b) {
	double a_offense = a->getOffenseTier();
	double a_defense = a->getDefenseTier();
	double b_offense = b->getOffenseTier();
	double b_defense = b->getDefenseTier();

	bool a_dies = false, b_dies = false;

	if (a_offense >= b_defense) {
		b_dies = true;
	}
	if (b_offense >= a_defense || b_defense >= a_offense) {
		a_dies = true;
	}

	if (a_dies && b_dies) {
		return -1;
	}
	if (a_dies) {
		return 0;
	}
	if (b_dies) {
		return 1;
	}
	return 2;
}