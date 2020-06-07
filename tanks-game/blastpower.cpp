#include "blastpower.h"
#include "blasttankpower.h"
#include "blastbulletpower.h"

const bool BlastPower::canBeInARandomLevel = true;

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
	//nope
}

BlastPower::~BlastPower() {
	//unlikely to ever need these
}
