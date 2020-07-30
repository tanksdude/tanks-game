#include "trackingbulletpower.h"
#include "trackingtankpower.h"
#include "powerfunctionhelper.h"
#include "constants.h"

InteractionBoolHolder TrackingBulletPower::modifiedMovement(Bullet* b) {
	if (PowerFunctionHelper::homingGeneric(b, 2*PI, false)) {
		//update with HomingBulletPower::modifiedMovement
		//can't just set b->angle to the correct angle because there has to be a targeting round (for multi-tank mode)
	}
	return { false };
}

InteractionBoolHolder TrackingBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	return { false, false };
}

void TrackingBulletPower::initialize(Bullet* parent) {
	//nothing
}

void TrackingBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* TrackingBulletPower::makeTankPower() const {
	return new TrackingTankPower();
}

TrackingBulletPower::TrackingBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;
}
