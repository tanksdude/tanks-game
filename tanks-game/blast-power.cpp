#include "blast-power.h"

const double BlastPower::bulletAngleDeviation = PI/3;
const int BlastPower::bulletAmount = 16;

const double BlastPower::maxBulletAcceleration = 3/16.0;
const double BlastPower::minBulletAcceleration = 1/16.0;
const double BlastPower::degradeAmount = .25;

std::unordered_map<std::string, float> BlastPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "random", .25f });
	return weights;
}

TankPower* BlastPower::makeTankPower() const {
	return new BlastTankPower();
}

BulletPower* BlastPower::makeBulletPower() const {
	return new BlastBulletPower();
}

/*
HazardPower* BlastPower::makeHazardPower() const {
	return new BlastHazardPower();
}
*/

Power* BlastPower::factory() {
	return new BlastPower();
}

BlastPower::BlastPower() {
	return;
}



#include "mylib.h"
#include "constants.h"
#include <cmath>
#include "rng.h"

void BlastTankPower::initialize(Tank* parent) {
	//nothing
}

void BlastTankPower::removeEffects(Tank* parent) {
	//nothing
}

void BlastTankPower::additionalShooting(Tank* t, const CannonPoint& c) {
	for (int i = 0; i < BlastPower::bulletAmount; i++) {
		double tempAngle = (RNG::randFunc()*2 - 1) * BlastPower::bulletAngleDeviation; //[-1,1) * deviation
		t->regularMakeBullet(t->r * cos(c.angle + t->velocity.getAngle() + tempAngle), t->r * sin(c.angle + t->velocity.getAngle() + tempAngle), c.angle + t->velocity.getAngle() + tempAngle);
	}
}

BulletPower* BlastTankPower::makeBulletPower() const {
	return new BlastBulletPower();
}

BlastTankPower::BlastTankPower() {
	maxTime = 500;
	timeLeft = 500;
	//JS: maxTime = 1000

	modifiesAdditionalShooting = true;
	overridesAdditionalShooting = true;
}



#include "collision-handler.h"
#include "rng.h"

InteractionBoolHolder BlastBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity.getMagnitude() <= 0) {
		b->opaqueness -= BlastPower::degradeAmount;
	} /*else if (b->velocity < 0) {
		b->velocity.setMagnitude(0);
		b->acceleration = 0;
	}*/
	return { false };
}

InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> BlastBulletPower::modifiedCollisionWithWall(const Bullet* b, const Wall* w) {
	if (b->velocity.getMagnitude() <= 0) {
		//b->opaqueness -= BlastPower::degradeAmount;
		return { b->isDead(), false, new BulletUpdateStruct(0,0,0,0,0, -BlastPower::degradeAmount), nullptr };
	} else {
		if (CollisionHandler::partiallyCollided(b, w)) {
			//CollisionHandler::pushMovableAwayFromImmovable(b, w);
			//TODO

			//b->acceleration = 0;
			//b->velocity.setMagnitude(0);
			return { false, false, new BulletUpdateStruct(0,0,0,0,0,0), nullptr };
		}
		return { false, false, nullptr, nullptr };
	}
}

bool BlastBulletPower::getModifiesCollisionWithCircleHazard(const CircleHazard* ch) const {
	return (ch->getCollisionType() == CircleHazardCollisionType::solid);
}

bool BlastBulletPower::getModifiesCollisionWithRectHazard(const RectHazard* rh) const {
	return (rh->getCollisionType() == RectHazardCollisionType::solid);
}

InteractionBoolHolder BlastBulletPower::modifiedCollisionWithCircleHazard(Bullet* b, CircleHazard* ch) {
	CollisionHandler::pushMovableAwayFromImmovable(b, ch);
	b->acceleration = 0;
	b->velocity.setMagnitude(0);
	return { false, false };
}

InteractionBoolHolder BlastBulletPower::modifiedCollisionWithRectHazard(Bullet* b, RectHazard* rh) {
	CollisionHandler::pushMovableAwayFromImmovable(b, rh);
	b->acceleration = 0;
	b->velocity.setMagnitude(0);
	return { false, false };
}

double BlastBulletPower::getBulletAcceleration() const {
	return accelerationAmount;
}

void BlastBulletPower::initialize(Bullet* parent) {
	//nothing
}

void BlastBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

BulletPower* BlastBulletPower::makeDuplicate() const {
	return new BlastBulletPower(this->accelerationAmount);
}

TankPower* BlastBulletPower::makeTankPower() const {
	return new BlastTankPower();
}

BlastBulletPower::BlastBulletPower()
: BlastBulletPower(-1 * ((RNG::randFunc()+RNG::randFunc())/2 * (BlastPower::maxBulletAcceleration - BlastPower::minBulletAcceleration) + BlastPower::minBulletAcceleration)) {}
//acceleration: [0,1) * accDiff + min

BlastBulletPower::BlastBulletPower(double acceleration) {
	timeLeft = 0;
	maxTime = -1;

	accelerationAmount = acceleration;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
}
