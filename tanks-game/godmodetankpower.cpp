#include "godmodetankpower.h"
#include "godmodebulletpower.h"
#include "powerupmanager.h"

void GodmodeTankPower::initialize(Tank* parent) {
	//this follows PowerSquare::givePower(Tank*)

	for (int i = 0; i < PowerupManager::getNumPowerTypes("supermix-vanilla"); i++) {
		Power* p = PowerupManager::getPowerFactory("supermix-vanilla", PowerupManager::getPowerName("supermix-vanilla", i))();
		parent->tankPowers.push_back(p->makeTankPower());
		parent->tankPowers[parent->tankPowers.size()-1]->initialize(parent);
		delete p;
	}
	parent->determineShootingAngles();
	parent->updateAllValues();
}

void GodmodeTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* GodmodeTankPower::makeBulletPower() const {
	return new GodmodeBulletPower();
}

GodmodeTankPower::GodmodeTankPower() {
	maxTime = 500;
	timeLeft = 500;
	//TODO: find power with longest time, then set this time to that
}
