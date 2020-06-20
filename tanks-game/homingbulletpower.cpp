#include "homingbulletpower.h"
#include "homingtankpower.h"
#include "powerfunctionhelper.h"
#include "constants.h"

const double HomingBulletPower::homingStrength = 2*PI / 256;

PowerInteractionBoolHolder HomingBulletPower::modifiedMovement(Bullet* b) {
	if (PowerFunctionHelper::homingGeneric(b, HomingBulletPower::homingStrength, true)) {
		//do another targeting round, but on hazards/"targetables" instead
		//this will only occur for some sort of "team mode" or single-player campaign
	}
	return { false };
}

PowerInteractionBoolHolder HomingBulletPower::modifiedEdgeCollision(Bullet* b) {
	return { b->x - b->r <= 0 || b->x + b->r >= GAME_WIDTH };
}

void HomingBulletPower::initialize(Bullet* parent) {
	//nothing
}

void HomingBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* HomingBulletPower::makeTankPower() {
	return new HomingTankPower();
}

HomingBulletPower::HomingBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	modifiesEdgeCollision = true;
}
