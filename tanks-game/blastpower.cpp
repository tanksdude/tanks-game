#include "blastpower.h"
#include "blasttankpower.h"
#include "blastbulletpower.h"

TankPower* BlastPower::makeTankPower() {
	return new BlastTankPower();
}

BulletPower* BlastPower::makeBulletPower() {
	return new BlastBulletPower();
}

/*
HazardPower* BlastPower::makeHazardPower(){
	return new BlastHazardPower();
}
*/

Power* BlastPower::factory() {
	return new BlastPower();
}

BlastPower::BlastPower() {
	return;
}
