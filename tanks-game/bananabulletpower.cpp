#include "bananabulletpower.h"
#include "bananatankpower.h"
#include "collisionhandler.h"
#include "bulletmanager.h"
#include "mylib.h"
#include "constants.h"

const int BananaBulletPower::bananaCount = 8;

void BananaBulletPower::modifiedMovement(Bullet* b) {
	//TODO: not done in the slightest
	if (b->velocity <= 0) {
		b->alpha = 0;
		for (int i = 0; i < bananaCount; i++) {
			BulletManager::pushBullet(new Bullet(b->x, b->y, b->r/2, randFunc() * 2*PI, 2, 0, b->getTeamID()));
		}
	}
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
