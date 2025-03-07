#include "god-mode-power.h"

#include "../constants.h"
#include <cmath> //fmod

#include "../color-mixer.h"
#include "../game-manager.h" //getTickCount()

std::unordered_map<std::string, float> GodmodePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "old", 0.5f });
	weights.insert({ "ultimate", 0.125f });
	weights.insert({ "ultimate-vanilla", 0.125f });
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

Power* GodmodePower::factory() {
	return new GodmodePower();
}

GodmodePower::GodmodePower() {
	return;
}



#include "../powerup-data-governor.h"

void GodmodeTankPower::initialize(Tank* parent) {
	//this follows PowerSquare::givePower(Tank*)

	for (int i = 0; i < PowerupDataGovernor::getNumPowerTypes("supermix-vanilla"); i++) {
		Power* p = PowerupDataGovernor::getPower("supermix-vanilla", PowerupDataGovernor::getPowerName("supermix-vanilla", i));
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
	//future note: it should probably be a fusion of every power instead of all powers separately applied
}



TankPower* GodmodeBulletPower::makeTankPower() const {
	return new GodmodeTankPower();
}

GodmodeBulletPower::GodmodeBulletPower() {
	timeLeft = 0;
	maxTime = -1;
}
