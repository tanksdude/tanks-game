#pragma once
class PowerFunctionHelper;

#include "tank.h"
#include "bullet.h"
#include "wall.h"

struct PowerFunctionHelper {
	static void wallhackGenericWall(Bullet*, Wall*);
	static void wallhackGenericTank(Tank*, Wall*);
	static void homingGeneric(Bullet*, double);
};