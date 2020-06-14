#include "bananabulletpower.h"
#include "bananatankpower.h"
#include "collisionhandler.h"
#include "bulletmanager.h"
#include "mylib.h"
#include "constants.h"
#include <vector>

const int BananaBulletPower::bananaCount = 8;

PowerInteractionBoolHolder BananaBulletPower::modifiedMovement(Bullet* b) {
	//TODO: somewhat done
	if (b->velocity <= 0) {
		std::vector<BulletPower*>* bp = new std::vector<BulletPower*>;
		bp->reserve(b->bulletPowers.size()-1);
		for (int i = 0; i < b->bulletPowers.size(); i++) {
			if (b->bulletPowers[i] != this) {
				bp->push_back(b->bulletPowers[i]);
			}
		}
		for (int i = 0; i < bananaCount; i++) {
			BulletManager::pushBullet(new Bullet(b->x, b->y, b->r/2 / b->getBulletRadiusMultiplier(), randFunc() * 2*PI, b->getInitialVelocity() / b->getBulletSpeedMultiplier(), b->getTeamID()));
		}
		delete bp;
		return { true };
	}
	return { false };
}

void BananaBulletPower::initialize(Bullet* b) {
	//nothing
}

void BananaBulletPower::removeEffects(Bullet* b) {
	//nothing
}

TankPower* BananaBulletPower::makeTankPower() {
	return new BananaTankPower();
}

BananaBulletPower::BananaBulletPower(){
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
}
