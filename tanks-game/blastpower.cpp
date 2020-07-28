#include "blastpower.h"
#include "blasttankpower.h"
#include "blastbulletpower.h"

TankPower* BlastPower::makeTankPower() const {
	return new BlastTankPower();
}

BulletPower* BlastPower::makeBulletPower() const {
	return new BlastBulletPower();
}

/*
HazardPower* BlastPower::makeHazardPower() const {
	return new BlastHazardPower();
}
*/

Power* BlastPower::factory() {
	return new BlastPower();
}

BlastPower::BlastPower() {
	return;
}
