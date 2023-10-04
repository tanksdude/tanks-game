#include "grenade-power.h"

const double GrenadePower::degradeAmount = .875; //JS: .75

std::unordered_map<std::string, float> GrenadePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "supermix", 1.0f });
	weights.insert({ "supermix-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

TankPower* GrenadePower::makeTankPower() const {
	return new GrenadeTankPower();
}

BulletPower* GrenadePower::makeBulletPower() const {
	return new GrenadeBulletPower();
}

/*
HazardPower* GrenadePower::makeHazardPower() const {
	return new GrenadeHazardPower();
}
*/

Power* GrenadePower::factory() {
	return new GrenadePower();
}

GrenadePower::GrenadePower() {
	return;
}



void GrenadeTankPower::initialize(Tank* parent) {
	//nothing
	//in JS, the tank's shooting cooldown was reset
}

void GrenadeTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* GrenadeTankPower::makeBulletPower() const {
	return new GrenadeBulletPower();
}

GrenadeTankPower::GrenadeTankPower() {
	maxTime = 500;
	timeLeft = 500;
	//JS: maxTime = 1000
}



InteractionBoolHolder GrenadeBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity.getMagnitude() <= 0) {
		b->lifeValue -= GrenadePower::degradeAmount;
		b->r *= 65/64.0;
	} /*else if (b->velocity.getMagnitude() < 0) {
		b->velocity.setMagnitude(0);
		b->acceleration = 0;
	}*/
	return { false };
}

InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> GrenadeBulletPower::modifiedCollisionWithWall(const Bullet* b, const Wall* w) {
	if (b->velocity.getMagnitude() <= 0) {
		//b->lifeValue -= GrenadePower::degradeAmount;
		return { b->isDead(), false, new BulletUpdateStruct(0,0,0,0,0, -GrenadePower::degradeAmount), nullptr };
	} else {
		if (b->acceleration < 0) {
			//b->velocity.changeMagnitude(b->acceleration);
			return { false, false, new BulletUpdateStruct(0,0,0, b->acceleration, 0,0), nullptr };
		}
		return { false, false, nullptr, nullptr };
	}
}

void GrenadeBulletPower::initialize(Bullet* parent) {
	//nothing
}

void GrenadeBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* GrenadeBulletPower::makeTankPower() const {
	return new GrenadeTankPower();
}

GrenadeBulletPower::GrenadeBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
}
