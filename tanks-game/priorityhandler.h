#pragma once
#include "tank.h"
#include "bullet.h"
#include "circlehazard.h"
#include "recthazard.h"

enum class PriorityResult {
	bothDie,
	firstDies,
	secondDies,
	neitherDie
};

class PriorityHandler {
public:
	static PriorityResult determinePriority(Tank* a, Tank* b);
	static PriorityResult determinePriority(Tank* a, Bullet* b);
	static PriorityResult determinePriority(Bullet* a, Bullet* b);

	static PriorityResult determinePriority(Tank* a, CircleHazard* b);
	static PriorityResult determinePriority(Tank* a, RectHazard* b);
	static PriorityResult determinePriority(Bullet* a, CircleHazard* b);
	static PriorityResult determinePriority(Bullet* a, RectHazard* b);

private:
	PriorityHandler() {}
	PriorityHandler(const PriorityHandler&) {}
};
