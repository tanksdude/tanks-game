#include "godmodebulletpower.h"
#include "godmodetankpower.h"

void GodmodeBulletPower::initialize(Bullet* parent) {
	//nothing
}

void GodmodeBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* GodmodeBulletPower::makeTankPower() const {
	return new GodmodeTankPower();
}

GodmodeBulletPower::GodmodeBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
