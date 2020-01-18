#pragma once
#include "bullet.h"

class BulletPriorityHandler {
private:
public:
	static char determinePriority(Bullet* a, Bullet* b); //<=-1 is both die, 0 is a dies, 1 is b dies, >=2 is neither dies
};