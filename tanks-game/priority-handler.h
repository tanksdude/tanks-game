#pragma once
#include "tank.h"
#include "bullet.h"
#include "circle-hazard.h"
#include "rect-hazard.h"

enum class PriorityResult {
	bothDie,
	firstDies,
	secondDies,
	neitherDie
};

class PriorityHandler {
public:
	static PriorityResult determinePriority(const Tank* a, const Tank* b);
	static PriorityResult determinePriority(const Tank* a, const Bullet* b);
	static PriorityResult determinePriority(const Bullet* a, const Bullet* b);

	static PriorityResult determinePriority(const Tank* a, const CircleHazard* b);
	static PriorityResult determinePriority(const Tank* a, const RectHazard* b);
	static PriorityResult determinePriority(const Bullet* a, const CircleHazard* b);
	static PriorityResult determinePriority(const Bullet* a, const RectHazard* b);

private:
	PriorityHandler() {}
	PriorityHandler(const PriorityHandler&) {}
};
