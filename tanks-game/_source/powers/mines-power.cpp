#include "mines-power.h"

const double MinesPower::bulletDistance = 1.0/8;

std::unordered_map<std::string, float> MinesPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 0.5f });
	weights.insert({ "old", 0.25f });
	return weights;
}

TankPower* MinesPower::makeTankPower() const {
	return new MinesTankPower();
}

BulletPower* MinesPower::makeBulletPower() const {
	return new MinesBulletPower();
}

/*
HazardPower* MinesPower::makeHazardPower() const {
	return new MinesHazardPower();
}
*/

Power* MinesPower::factory() {
	return new MinesPower();
}

MinesPower::MinesPower() {
	return;
}



#include "../constants.h"
#include <cmath>

void MinesTankPower::tick(Tank* t) {
	for (int i = 0; i < t->tankPowers.size(); i++) {
		if (t->tankPowers[i] != this) [[likely]] {
			if (t->tankPowers[i]->modifiesAdditionalShooting) {
				//might also want to check addsShootingPoints and/or addsExtraShootingPoints
				this->modifiesAdditionalShooting = false;
				this->overridesAdditionalShooting = false;
				return;
			}
		}
	}
	this->modifiesAdditionalShooting = true;
	this->overridesAdditionalShooting = true;
}

void MinesTankPower::additionalShooting(Tank* t, const CannonPoint& c, const ExtraCannonPoint& c2) {
	t->preciseMakeBullet((t->r * MinesPower::bulletDistance) * cos(t->velocity.getAngle() + c.angleFromCenter + c2.angleFromCenter),
	                     (t->r * MinesPower::bulletDistance) * sin(t->velocity.getAngle() + c.angleFromCenter + c2.angleFromCenter),
	                     t->velocity.getAngle() + c.angleFromCenter + c2.angleFromCenter + c2.angleFromEdge);
}

BulletPower* MinesTankPower::makeBulletPower() const {
	return new MinesBulletPower();
}

MinesTankPower::MinesTankPower() {
	maxTime = 500;
	timeLeft = 500;
	//in JS, the tank's shooting cooldown was reset

	modifiesAdditionalShooting = true;
	overridesAdditionalShooting = true;
}



TankPower* MinesBulletPower::makeTankPower() const {
	return new MinesTankPower();
}

MinesBulletPower::MinesBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
