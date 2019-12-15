#pragma once
struct PowerFunctionHelper;

#include <vector>
#include "tank.h"
#include "cannonpoint.h"
#include "bullet.h"
#include "wall.h"

struct PowerFunctionHelper {
	static bool wallhackGenericBullet(Bullet*, Wall*);
	static bool wallhackGenericTank(Tank*, Wall*);
	static bool homingGeneric(Bullet*, double);
	static bool bounceGeneric(Bullet*, Wall*);
	static bool bounceGenericWithCorners(Bullet*, Wall*);
	static bool bounceEdgeGeneric(Bullet*);
	static void equallySpacedCannonPoints(Tank*, std::vector<CannonPoint>*, short); //fix?
};