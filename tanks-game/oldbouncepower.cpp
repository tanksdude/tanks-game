#include "oldbouncepower.h"

std::unordered_map<std::string, float> OldBouncePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
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



#include "powerfunctionhelper.h"
#include "collisionhandler.h"

//const int OldBounceBulletPower::maxBounces = 16;

/*
InteractionBoolHolder OldBounceBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
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

	bouncesLeft = OldBounceBulletPower::maxBounces;

	modifiesCollisionWithWall = true;
	//modifiedCollisionWithWallCanWorkWithOthers = false;
	modifiesEdgeCollision = true;
	*/
}
