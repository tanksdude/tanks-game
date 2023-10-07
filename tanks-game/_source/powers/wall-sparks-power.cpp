#include "wall-sparks-power.h"

#include "../constants.h"

#include "bounce-power.h"

const int WallSparksPower::extraBulletsCount = 4;
const int WallSparksPower::maxBounces = BouncePower::maxBounces;
const double WallSparksPower::maxNewBulletVelocity = 1.25;
const double WallSparksPower::minNewBulletVelocity =  .75;
const double WallSparksPower::bulletAngleDeviation = PI/8;

std::unordered_map<std::string, float> WallSparksPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

TankPower* WallSparksPower::makeTankPower() const {
	return new WallSparksTankPower();
}

BulletPower* WallSparksPower::makeBulletPower() const {
	return new WallSparksBulletPower();
}

/*
HazardPower* WallSparksPower::makeHazardPower() const {
	return new WallSparksHazardPower();
}
*/

Power* WallSparksPower::factory() {
	return new WallSparksPower();
}

WallSparksPower::WallSparksPower() {
	return;
}



void WallSparksTankPower::initialize(Tank* parent) {
	//nothing
}

void WallSparksTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* WallSparksTankPower::makeBulletPower() const {
	return new WallSparksBulletPower();
}

WallSparksTankPower::WallSparksTankPower() {
	maxTime = 500;
	timeLeft = 500;
}



#include <vector>
#include "../rng.h"

#include "../power-function-helper.h"
#include "../bullet-manager.h"

InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> WallSparksBulletPower::modifiedCollisionWithWall(const Bullet* b, const Wall* w) {
	std::shared_ptr<BulletUpdateStruct> b_update;
	std::shared_ptr<WallUpdateStruct> w_update;

	//first, bounce off wall

	auto result = PowerFunctionHelper::bounceGeneric(b, w);
	b_update = result.second.firstUpdate;
	w_update = result.second.secondUpdate;
	if (result.first) {
		bouncesLeft--;
	}

	if (bouncesLeft <= 0) {
		modifiesCollisionWithWall = false;
	}

	//second, create more bullets (they lose this power, just like banana)

	for (int i = 0; i < WallSparksPower::extraBulletsCount; i++) {
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
		newVelocity = newVelocity * ((RNG::randFunc()+RNG::randFunc())/2 * (WallSparksPower::maxNewBulletVelocity - WallSparksPower::minNewBulletVelocity) + WallSparksPower::minNewBulletVelocity);
		double newAngle = (b->velocity.getAngle() + b_update->angle) + (RNG::randFunc()+RNG::randFunc() - 1) * WallSparksPower::bulletAngleDeviation;
		BulletManager::pushBullet(new Bullet(b->x + b_update->x, b->y + b_update->y, b->r, newAngle, newVelocity, b->getTeamID(), b->getParentIDType(), b->getParentID(), bp, true));
		delete bp;
	}

	return { (bouncesLeft < 0), false, b_update, w_update };
	//feels a little overpowered to let the bullet bounce...
}

void WallSparksBulletPower::initialize(Bullet* parent) {
	//nothing
}

void WallSparksBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* WallSparksBulletPower::makeTankPower() const {
	return new WallSparksTankPower();
}

WallSparksBulletPower::WallSparksBulletPower() : WallSparksBulletPower(WallSparksPower::maxBounces) {}

WallSparksBulletPower::WallSparksBulletPower(int bounces) {
	timeLeft = 0;
	maxTime = -1;

	bouncesLeft = bounces;
	if (bounces > 0) {
		modifiesCollisionWithWall = true;
	}
}
