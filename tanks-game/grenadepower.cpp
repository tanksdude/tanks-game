#include "grenadepower.h"

std::unordered_map<std::string, float> GrenadePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "supermix", .5f });
	weights.insert({ "supermix-vanilla", .5f });
	weights.insert({ "random", .5f });
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



const double GrenadeBulletPower::degradeAmount = .875;

InteractionBoolHolder GrenadeBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity.getMagnitude() <= 0) {
		b->opaqueness -= degradeAmount;
		b->r *= 65/64.0;
	} /*else if (b->velocity.getMagnitude() < 0) {
		b->velocity.setMagnitude(0);
		b->acceleration = 0;
	}*/
	return { false };
}

InteractionBoolHolder GrenadeBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (b->velocity.getMagnitude() <= 0) {
		b->opaqueness -= degradeAmount;
		return { b->isDead(), false };
	} else {
		if (b->acceleration < 0) {
			b->velocity.changeMagnitude(b->acceleration);
			/*
			if (b->velocity.getMagnitude() < 0) {
				b->velocity.setMagnitude(0);
			}
			*/
		}
		return { false, false };
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
