#include "godmodepower.h"
#include "colormixer.h"
#include "gamemanager.h"

std::unordered_map<std::string, float> GodmodePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "ultimate", .125f });
	weights.insert({ "ultimate-vanilla", .125f });
	return weights;
}

ColorValueHolder GodmodePower::getClassColor() {
	return ColorMixer::HSVtoRGB(fmod(GameManager::getTickCount(), 360), .75, .75);
	//JS just did the tank default color (as a PowerSquare; it was rainbow for tanks and bullets)
}

TankPower* GodmodePower::makeTankPower() const {
	return new GodmodeTankPower();
}

BulletPower* GodmodePower::makeBulletPower() const {
	return new GodmodeBulletPower();
}

/*
HazardPower* GodmodePower::makeHazardPower() const {
	return new GodmodeHazardPower();
}
*/

Power* GodmodePower::factory() {
	return new GodmodePower();
}

GodmodePower::GodmodePower() {
	return;
}



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
