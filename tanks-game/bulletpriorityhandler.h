#pragma once
#include "bullet.h"

class BulletPriorityHandler {
private:
	//static short getNextHighestTier(Bullet* z, short current_tier);
public:
	static Bullet* determinePriority(Bullet* a, Bullet* b); //returns whichever wins; returns null if equal and both die, junk bullet if both live
};