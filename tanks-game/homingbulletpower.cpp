#pragma once
#include "homingbulletpower.h"
#include "homingtankpower.h"
#include "homingpower.h"
#include "powerfunctionhelper.h"
#include <math.h>
#include "constants.h"

const double HomingBulletPower::homingStrength = 2*PI / 256;

void HomingBulletPower::modifiedMovement(Bullet* b) {
	if (PowerFunctionHelper::homingGeneric(b, HomingBulletPower::homingStrength, true)) {
		//do another targeting round, but on hazards/"targetables" instead
	}
}

bool HomingBulletPower::modifiedEdgeCollision(Bullet* b) {
	return (b->x - b->r <= 0 || b->x + b->r >= GAME_WIDTH);
}

void HomingBulletPower::initialize(Bullet* b) {
	//nothing
}

void HomingBulletPower::removeEffects(Bullet * b) {
	//nothing
}

TankPower* HomingBulletPower::makeTankPower() {
	return new HomingTankPower();
}

HomingBulletPower::HomingBulletPower(){
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	modifiesCollisionWithEdge = true;
}