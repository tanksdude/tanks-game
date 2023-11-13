#include "grenade-power.h"

const double GrenadePower::degradeAmount = .875; //JS: .75
const double GrenadePower::growAmount = 65/64.0;

std::unordered_map<std::string, float> GrenadePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 0.5f });
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



BulletPower* GrenadeTankPower::makeBulletPower() const {
	return new GrenadeBulletPower();
}

GrenadeTankPower::GrenadeTankPower() {
	maxTime = 500;
	timeLeft = 500;
	//JS: maxTime = 1000
	//in JS, the tank's shooting cooldown was reset
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

TankPower* GrenadeBulletPower::makeTankPower() const {
	return new GrenadeTankPower();
}

GrenadeBulletPower::GrenadeBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;

	bulletRadiusGrowMultiplies_Stationary = true;
}
