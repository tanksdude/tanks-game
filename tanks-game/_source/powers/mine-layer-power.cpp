#include "mine-layer-power.h"

const double MineLayerPower::tickCycle = 50;

std::unordered_map<std::string, float> MineLayerPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

TankPower* MineLayerPower::makeTankPower() const {
	return new MineLayerTankPower();
}

BulletPower* MineLayerPower::makeBulletPower() const {
	return new MineLayerBulletPower();
}

Power* MineLayerPower::factory() {
	return new MineLayerPower();
}

MineLayerPower::MineLayerPower() {
	return;
}



BulletPower* MineLayerTankPower::makeBulletPower() const {
	return new MineLayerBulletPower();
}

MineLayerTankPower::MineLayerTankPower() {
	maxTime = 500;
	timeLeft = 500;
}



#include "../constants.h"
#include <cmath>

#include "../bullet-manager.h"

void MineLayerBulletPower::initialize(Bullet* parent) {
	this->last_xpos = parent->getX();
	this->last_ypos = parent->getY();
}

void MineLayerBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

void MineLayerBulletPower::layMine(const Bullet* b) {
	std::vector<BulletPower*>* bp = new std::vector<BulletPower*>;
	bp->reserve(b->bulletPowers.size()-1);
	for (int i = 0; i < b->bulletPowers.size(); i++) {
		if (b->bulletPowers[i] != this) [[likely]] {
			bp->push_back(b->bulletPowers[i]->makeDuplicate());
		}
	}
	BulletManager::pushBullet(new Bullet(b->x, b->y, b->r, b->velocity.getAngle(), 0, b->getTeamID(), b->getParentIDType(), b->getParentID(), bp, true));
	delete bp;
	//note: this means the new bullet is on top of the parent; oh well, this is so far literally the only power where that's a problem
}

InteractionBoolHolder MineLayerBulletPower::modifiedMovement(Bullet* b) {
	//bool laidMine = false;
	if (tickCount >= MineLayerPower::tickCycle) {
		if ((b->getX() == this->last_xpos) && (b->getY() == this->last_ypos)) {
			return { false };
			//quit early to avoid tickCount++
		}
		layMine(b);
		tickCount -= MineLayerPower::tickCycle;
	}

	tickCount++; //not sure whether this should go at the start or end
	return { false };
}

TankPower* MineLayerBulletPower::makeTankPower() const {
	return new MineLayerTankPower();
}

MineLayerBulletPower::MineLayerBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;

	tickCount = 0;

	//last_xpos and last_ypos not set, that happens in initialize()
}
