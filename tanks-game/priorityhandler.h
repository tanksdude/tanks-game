#pragma once
#include "tank.h"
#include "bullet.h"

class PriorityHandler {
private:
public:
	static char determinePriority(Bullet* a, Bullet* b); //-1 is both die, 0 is a dies, 1 is b dies, >=2 is neither dies, <=-2 is "random" (not used)
	static char determinePriority(Bullet* a, Tank* b); //-1 is both die, 0 is a dies, 1 is b dies, >=2 is neither dies, <=-2 is "random" (not used)
	//^ should probably be moved...
};