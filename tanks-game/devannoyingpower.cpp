#include "devannoyingpower.h"

TankPower* DevAnnoyingPower::makeTankPower() const {
	return new DevAnnoyingTankPower();
}

BulletPower* DevAnnoyingPower::makeBulletPower() const {
	return new DevAnnoyingBulletPower();
}

/*
HazardPower* DevAnnoyingPower::makeHazardPower() const {
	return new DevAnnoyingHazardPower();
}
*/

Power* DevAnnoyingPower::factory() {
	return new DevAnnoyingPower();
}

DevAnnoyingPower::DevAnnoyingPower() {
	return;
}



#include "collisionhandler.h"

void DevAnnoyingTankPower::initialize(Tank* parent) {
	//nothing
}

void DevAnnoyingTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* DevAnnoyingTankPower::makeBulletPower() const {
	return new DevAnnoyingBulletPower();
}

InteractionBoolHolder DevAnnoyingTankPower::modifiedEdgeCollision(Tank* parent) {
	//this isn't annoying per se, I just wanted to do it
	//keep the tank in-bounds, but move "in-bounds" one tank diameter away
	CollisionHandler::edgeConstrain(parent, parent->getR() * -2);

	return { false };
}

DevAnnoyingTankPower::DevAnnoyingTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesEdgeCollision = true;
}



#include "collisionhandler.h"

void DevAnnoyingBulletPower::initialize(Bullet* parent) {
	//nothing
}

void DevAnnoyingBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* DevAnnoyingBulletPower::makeTankPower() const {
	return new DevAnnoyingTankPower();
}

InteractionBoolHolder DevAnnoyingBulletPower::modifiedCollisionWithTank(Bullet* parent, Tank* t) {
	if (CollisionHandler::partiallyCollided(parent, t)) {
		CollisionHandler::pushMovableAwayFromMovable(parent, t);
	}
	return { false, false };
}

//has very low offense, very high defense; can't kill, can't be killed
double DevAnnoyingBulletPower::getOffenseImportance() const {
	return .5; //0?
}

double DevAnnoyingBulletPower::getOffenseTier(Bullet*) const {
	return 0; //-1? //not sure
}

double DevAnnoyingBulletPower::getDefenseImportance() const {
	return 0; //.5?
}

double DevAnnoyingBulletPower::getDefenseTier(Bullet*) const {
	return 0; //999? //should be <= 0 so other bullets can kill it... right?
}

DevAnnoyingBulletPower::DevAnnoyingBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithTank = true;
	//modifiedCollisionWithTankCanWorkWithOthers = false; //TODO: should this be set?
}
