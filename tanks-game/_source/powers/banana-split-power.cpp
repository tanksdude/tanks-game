#include "banana-split-power.h"

const int BananaSplitPower::bananaSplitCount = 4;
const double BananaSplitPower::bananaSplitAngleDeviation = PI/4;
static_assert(BananaSplitPower::bananaSplitCount > 1);

std::unordered_map<std::string, float> BananaSplitPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

TankPower* BananaSplitPower::makeTankPower() const {
	return new BananaSplitTankPower();
}

BulletPower* BananaSplitPower::makeBulletPower() const {
	return new BananaSplitBulletPower();
}

/*
HazardPower* BananaSplitPower::makeHazardPower() const {
	return new BananaSplitHazardPower();
}
*/

Power* BananaSplitPower::factory() {
	return new BananaSplitPower();
}

BananaSplitPower::BananaSplitPower() : BananaPower() {
	return;
}



BulletPower* BananaSplitTankPower::makeBulletPower() const {
	return new BananaSplitBulletPower();
}

BananaSplitTankPower::BananaSplitTankPower() : BananaTankPower() {
	//nothing new
}



#include "../constants.h"
#include <vector>
#include "../rng.h"

#include "../bullet-manager.h"

void BananaSplitBulletPower::bananaExplode(const Bullet* b) {
	//copied from BananaBulletPower::bananaExplode(), *slightly* modified
	for (int i = 0; i < BananaSplitPower::bananaSplitCount; i++) {
		std::vector<BulletPower*>* bp = new std::vector<BulletPower*>;
		bp->reserve(b->bulletPowers.size()-1);
		for (int i = 0; i < b->bulletPowers.size(); i++) {
			if (b->bulletPowers[i] != this) {
				bp->push_back(b->bulletPowers[i]->makeDuplicate());
			}
		}
		double newVelocity = b->getInitialVelocity();
		if (newVelocity == 0) {
			newVelocity = Tank::default_maxSpeed*BULLET_TO_TANK_SPEED_RATIO; //if bullet's initial speed is zero, it should still explode (TODO: what should the initial speed be?)
		}
		newVelocity = newVelocity * ((RNG::randFunc()+RNG::randFunc())/2 * (BananaPower::maxNewBulletVelocityMultiplier - BananaPower::minNewBulletVelocityMultiplier) + BananaPower::minNewBulletVelocityMultiplier);
		const double newAngle = b->velocity.getAngle() + ((double(i)/(BananaSplitPower::bananaSplitCount-1)) * (2*BananaSplitPower::bananaSplitAngleDeviation) - BananaSplitPower::bananaSplitAngleDeviation); //[-1,1) * deviation
		BulletManager::pushBullet(new Bullet(b->x, b->y, b->r/2, newAngle, newVelocity, b->getTeamID(), b->getParentIDType(), b->getParentID(), bp, true));
		delete bp;
	}
}

TankPower* BananaSplitBulletPower::makeTankPower() const {
	return new BananaSplitTankPower();
}

BananaSplitBulletPower::BananaSplitBulletPower() : BananaBulletPower() {
	//nothing new
}
