#pragma once
#include "powerfunctionhelper.h"
#include "tank.h"
#include "bullet.h"
#include "wall.h"

bool PowerFunctionHelper::wallhackGenericBullet(Bullet*, Wall*) {
	return false;
}

bool PowerFunctionHelper::wallhackGenericTank(Tank*, Wall*) {
	return false;
}

bool PowerFunctionHelper::homingGeneric(Bullet*, double) {
	//fix: deal with later
	return false;
}
