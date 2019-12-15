#pragma once
struct PowerFunctionHelper;

#include "tank.h"
#include "bullet.h"
#include "wall.h"

struct PowerFunctionHelper {
	static bool wallhackGenericBullet(Bullet*, Wall*);
	static bool wallhackGenericTank(Tank*, Wall*);
	static bool homingGeneric(Bullet*, double);
};