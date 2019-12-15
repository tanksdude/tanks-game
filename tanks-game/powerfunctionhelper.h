#pragma once
struct PowerFunctionHelper;

#include <vector>
#include "tank.h"
#include "cannonpoint.h"
#include "bullet.h"
#include "wall.h"

struct PowerFunctionHelper {
private:
	static bool bounceGenericWithCornersCornerHandler(Bullet*, double x, double y);

public:
	static bool wallhackGenericBullet(Bullet*, Wall*);
	static bool wallhackGenericTank(Tank*, Wall*);
	//can combine the wallhacks to take a Circle* but not a big deal
	static bool homingGeneric(Bullet*, double);
	static bool bounceGeneric(Bullet*, Wall*);
	static bool bounceGenericWithCorners(Bullet*, Wall*);
	static bool bounceEdgeGenericX(Bullet*);
	static bool bounceEdgeGenericY(Bullet*);
	static void equallySpacedCannonPoints(Tank*, std::vector<CannonPoint>*, short); //fix?
};