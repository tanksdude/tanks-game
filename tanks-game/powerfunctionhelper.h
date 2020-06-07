#pragma once
#include "tank.h"
#include <vector>
#include "cannonpoint.h"
#include "bullet.h"
#include "wall.h"

class PowerFunctionHelper {
private:
	static bool bounceGenericWithCornersCornerHandler(Bullet*, double x, double y);

public:
	static bool homingGeneric(Bullet*, double, bool moveByAngle);
	static bool bounceGeneric(Bullet*, Wall*);
	static bool bounceGenericWithCorners(Bullet*, Wall*);
	static bool bounceEdgeGenericX(Bullet*);
	static bool bounceEdgeGenericY(Bullet*);
	static void equallySpacedCannonPoints(Tank*, std::vector<CannonPoint>*, short); //fix?
};
