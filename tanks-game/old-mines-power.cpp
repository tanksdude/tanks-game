#include "old-mines-power.h"

std::unordered_map<std::string, float> OldMinesPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", .25f });
	return weights;
}

TankPower* OldMinesPower::makeTankPower() const {
	return new OldMinesTankPower();
}

BulletPower* OldMinesPower::makeBulletPower() const {
	return new OldMinesBulletPower();
}

/*
HazardPower* OldMinesPower::makeHazardPower() const {
	return new OldMinesHazardPower();
}
*/

Power* OldMinesPower::factory() {
	return new OldMinesPower();
}

OldMinesPower::OldMinesPower() : MinesPower() {
	return;
}



//#include "constants.h"
#include <math.h>

//const double OldMinesTankPower::bulletDistance = 1.0/8; //use MinesTankPower for this

/*
void OldMinesTankPower::tick(Tank* t) {
	for (int i = 0; i < t->tankPowers.size(); i++) {
		if (t->tankPowers[i] != this) {
			if (t->tankPowers[i]->modifiesAdditionalShooting) {
				//might also want to check addsShootingPoints
				this->modifiesAdditionalShooting = false;
				return;
			}
		}
	}
	this->modifiesAdditionalShooting = true;
}

void MinesTankPower::additionalShooting(Tank* t, CannonPoint c) {
	t->regularMakeBullet(t->r * cos(t->velocity.getAngle() + c.angle) * bulletDistance, t->r * sin(t->velocity.getAngle() + c.angle) * bulletDistance, c.angle + t->velocity.getAngle());
}
*/

void OldMinesTankPower::initialize(Tank* parent) {
	//nothing
	//in JS, the tank's shooting cooldown was reset
}

void OldMinesTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* OldMinesTankPower::makeBulletPower() const {
	return new OldMinesBulletPower();
}

OldMinesTankPower::OldMinesTankPower() : MinesTankPower() {
	/*
	maxTime = 500;
	timeLeft = 500;

	modifiesAdditionalShooting = true;
	overridesAdditionalShooting = true;
	*/

	tankFiringRateStacks = true;
}



void OldMinesBulletPower::initialize(Bullet* parent) {
	//nothing
}

void OldMinesBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* OldMinesBulletPower::makeTankPower() const {
	return new OldMinesTankPower();
}

OldMinesBulletPower::OldMinesBulletPower() : MinesBulletPower() {
	/*
	timeLeft = 0;
	maxTime = -1;
	*/
}
