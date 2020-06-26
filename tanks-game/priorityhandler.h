#pragma once
#include "tank.h"
#include "bullet.h"
#include "circlehazard.h"
#include "recthazard.h"

class PriorityHandler {
private:
public:
	//-1 is both die, 0 is a dies, 1 is b dies, >=2 is neither dies, <=-2 is "random" (not used)
	//priority between something and a tank takes some extra consideration

	static char determinePriority(Tank* a, Tank* b);
	static char determinePriority(Tank* a, Bullet* b);
	static char determinePriority(Bullet* a, Bullet* b);

	static char determinePriority(Tank* a, CircleHazard* b);
	static char determinePriority(Tank* a, RectHazard* b);
	static char determinePriority(Bullet* a, CircleHazard* b);
	static char determinePriority(Bullet* a, RectHazard* b);
};
