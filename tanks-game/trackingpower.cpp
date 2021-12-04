#include "trackingpower.h"

std::unordered_map<std::string, float> TrackingPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .25f });
	weights.insert({ "old", .25f });
	weights.insert({ "random-old", .125f });
	return weights;
}

TankPower* TrackingPower::makeTankPower() const {
	return new TrackingTankPower();
}

BulletPower* TrackingPower::makeBulletPower() const {
	return new TrackingBulletPower();
}

/*
HazardPower* TrackingPower::makeHazardPower() const {
	return new TrackingHazardPower();
}
*/

Power* TrackingPower::factory() {
	return new TrackingPower();
}

TrackingPower::TrackingPower() {
	return;
}



void TrackingTankPower::initialize(Tank* parent) {
	//nothing
}

void TrackingTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* TrackingTankPower::makeBulletPower() const {
	return new TrackingBulletPower();
}

TrackingTankPower::TrackingTankPower() {
	maxTime = 500;
	timeLeft = 500;
}



#include "powerfunctionhelper.h"
#include "constants.h"

InteractionBoolHolder TrackingBulletPower::modifiedMovement(Bullet* b) {
	Game_ID targetID = PowerFunctionHelper::homingGenericTarget(b, true);
	if (targetID != -1) {
		PowerFunctionHelper::homingGenericMove(b, targetID, 2*PI);
	} else {
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
