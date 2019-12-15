#pragma once
#include "powerfunctionhelper.h"
#include "tank.h"
#include "bullet.h"
#include "wall.h"

void PowerFunctionHelper::wallhackGenericWall(Bullet*, Wall*) {
	return;
}

void PowerFunctionHelper::wallhackGenericTank(Tank*, Wall*) {
	return;
}

void PowerFunctionHelper::homingGeneric(Bullet*, double) {
	return; //fix: deal with later
}
