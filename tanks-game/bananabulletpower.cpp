#include "bananabulletpower.h"
#include "bananatankpower.h"
#include "collisionhandler.h"
#include "bulletmanager.h"
#include "mylib.h"
#include "constants.h"
#include <vector>

const int BananaBulletPower::bananaCount = 8;

PowerInteractionBoolHolder BananaBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity <= 0) {
		for (int i = 0; i < bananaCount; i++) {
			std::vector<BulletPower*>* bp = new std::vector<BulletPower*>;
			bp->reserve(b->bulletPowers.size()-1);
			for (int i = 0; i < b->bulletPowers.size(); i++) {
				if (b->bulletPowers[i] != this) {
					bp->push_back(b->bulletPowers[i]->makeDuplicate());
				}
			}
			BulletManager::pushBullet(new Bullet(b->x, b->y, b->r/2, randFunc() * 2*PI, b->getInitialVelocity(), b->getTeamID(), bp, true));
			delete bp;
		}
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
