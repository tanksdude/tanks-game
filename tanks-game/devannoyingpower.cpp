#include "devannoyingpower.h"

std::unordered_map<std::string, float> DevAnnoyingPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f }); //common since its purpose is to show off game underworkings
	weights.insert({ "random-dev", 0.5f }); //... it's annoying, don't overuse it
	return weights;
}

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
	parent->velocity.setMagnitude(0);
	//it's annoying
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

double DevAnnoyingTankPower::getTankAccelerationMultiplier() const {
	//might change this
	return .5;
}

DevAnnoyingTankPower::DevAnnoyingTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesEdgeCollision = true;
}



#include "collisionhandler.h"
//#include "constants.h"

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
//^^^ cancelled but might reinvestigate in the future (TODO)

double DevAnnoyingBulletPower::getOffenseImportance() const {
	return .5; //0?
}

double DevAnnoyingBulletPower::getOffenseTier(const Bullet*) const {
	return 0; //LOW_TIER? //not sure
}

double DevAnnoyingBulletPower::getDefenseImportance() const {
	return 0; //.5?
}

double DevAnnoyingBulletPower::getDefenseTier(const Bullet*) const {
	return 0; //HIGH_TIER? //should be <= 0 so other bullets can kill it... right?
}

DevAnnoyingBulletPower::DevAnnoyingBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithTank = true;
	//modifiedCollisionWithTankCanWorkWithOthers = false; //TODO: should this be set?
}
