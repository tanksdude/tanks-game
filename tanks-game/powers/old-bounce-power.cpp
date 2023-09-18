#include "old-bounce-power.h"

//const int OldBouncePower::maxBounces = 16;

std::unordered_map<std::string, float> OldBouncePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	return weights;
}

TankPower* OldBouncePower::makeTankPower() const {
	return new OldBounceTankPower();
}

BulletPower* OldBouncePower::makeBulletPower() const {
	return new OldBounceBulletPower();
}

/*
HazardPower* OldBouncePower::makeHazardPower() const {
	return new OldBounceHazardPower();
}
*/

Power* OldBouncePower::factory() {
	return new OldBouncePower();
}

OldBouncePower::OldBouncePower() : BouncePower() {
	return;
}



void OldBounceTankPower::initialize(Tank* parent) {
	//nothing
}

void OldBounceTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* OldBounceTankPower::makeBulletPower() const {
	return new OldBounceBulletPower();
}

OldBounceTankPower::OldBounceTankPower() : BounceTankPower() {
	/*
	maxTime = 500;
	timeLeft = 500;
	*/
}



#include "../collision-handler.h"
#include "../power-function-helper.h"

/*
InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> OldBounceBulletPower::modifiedCollisionWithWall(const Bullet* b, const Wall* w) {
	return BounceBulletPower::modifiedCollisionWithWall(b, w);
}

InteractionBoolHolder OldBounceBulletPower::modifiedEdgeCollision(Bullet* b) {
	return BounceBulletPower::modifiedEdgeCollision(b);
}
*/

void OldBounceBulletPower::initialize(Bullet* parent) {
	//nothing
}

void OldBounceBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* OldBounceBulletPower::makeTankPower() const {
	return new OldBounceTankPower();
}

OldBounceBulletPower::OldBounceBulletPower() : BounceBulletPower() {
	/*
	timeLeft = 0;
	maxTime = -1;

	bouncesLeft = OldBouncePower::maxBounces;

	modifiesCollisionWithWall = true;
	//modifiedCollisionWithWallCanWorkWithOthers = false;
	modifiesEdgeCollision = true;
	*/
}