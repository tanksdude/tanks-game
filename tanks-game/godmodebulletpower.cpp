#include "godmodebulletpower.h"
#include "godmodetankpower.h"

void GodmodeBulletPower::initialize(Bullet* b) {
	//nothing
}

void GodmodeBulletPower::removeEffects(Bullet* b) {
	//nothing
}

TankPower* GodmodeBulletPower::makeTankPower() {
	return new GodmodeTankPower();
}

GodmodeBulletPower::GodmodeBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
