#include "shield-power.h"
#include "../game-manager.h" //settings

const double ShieldPower::barrierStrength = 50; //TODO: depend on global power duration

std::unordered_map<std::string, float> ShieldPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 0.5f });
	return weights;
}

//process of narrowing down the specific color:
/*
//static ColorValueHolder getClassColor() { return ColorValueHolder(0.625f, 0.75f, 0.9375f); } //very light blue (baby blue?)
//static ColorValueHolder getClassColor() { return ColorValueHolder(0.0625f, 0.375f, 0.6875f); } //sapphire
//static ColorValueHolder getClassColor() { return ColorValueHolder(47/255.0, 117/255.0, 187/255.0); } //kinda sapphire
//static ColorValueHolder getClassColor() { return ColorValueHolder(100/255.0, 149/255.0, 222/255.0); } //kinda sapphire, again
//static ColorValueHolder getClassColor() { return ColorValueHolder(116/255.0, 161/255.0, 229/255.0); } //less blue (good, but now I've decided on green)
//static ColorValueHolder getClassColor() { return ColorValueHolder(161/255.0, 229/255.0, 116/255.0); } //green equivalent
//static ColorValueHolder getClassColor() { return ColorValueHolder(116/255.0, 229/255.0, 127/255.0); } //lil' more cyan
//static ColorValueHolder getClassColor() { return ColorValueHolder(0xA7/255.0, 0xE0/255.0, 0xA3/255.0); } //new approach, more gray
static ColorValueHolder getClassColor() { return ColorValueHolder(0xB6/255.0, 0xE6/255.0, 0x9D/255.0); } //less gray, shift towards yellow (good)
//static ColorValueHolder getClassColor() { return ColorValueHolder(0xB3/255.0, 0xF3/255.0, 0x90/255.0); } //less gray
//static ColorValueHolder getClassColor() { return ColorValueHolder(0x94/255.0, 0xEE/255.0, 0x63/255.0); } //turn down light
//static ColorValueHolder getClassColor() { return ColorValueHolder(0x9A/255.0, 0xD8/255.0, 0x79/255.0); } //more gray
//static ColorValueHolder getClassColor() { return ColorValueHolder(0x80/255.0, 0xDD/255.0, 0x80/255.0); } //restart: life but *really* white
//static ColorValueHolder getClassColor() { return ColorValueHolder(0x98/255.0, 0xC8/255.0, 0x80/255.0); } //less green, shift yellow
*/

TankPower* ShieldPower::makeTankPower() const {
	return new ShieldTankPower();
}

BulletPower* ShieldPower::makeBulletPower() const {
	return new ShieldBulletPower();
}

Power* ShieldPower::factory() {
	return new ShieldPower();
}

ShieldPower::ShieldPower() {
	return;
}



TankPower* ShieldTankPower::makeDuplicate() const {
	return new ShieldTankPower(this->timeLeft);
}

BulletPower* ShieldTankPower::makeBulletPower() const {
	return new ShieldBulletPower();
}

InteractionBoolHolder ShieldTankPower::modifiedDeathHandling(Tank* parent) {
	if (this->maxTime < 0) {
		return { false, false };
	}
	if (this->timeLeft <= ShieldPower::barrierStrength) {
		return { false, true };
	}
	this->timeLeft -= ShieldPower::barrierStrength;
	return { false, false };
}

ShieldTankPower::ShieldTankPower() : ShieldTankPower(GameManager::get_settings().PowerupDurationBaseTime) {}

ShieldTankPower::ShieldTankPower(double life) {
	maxTime = life;
	timeLeft = life;

	modifiesDeathHandling = true;
}



BulletPower* ShieldBulletPower::makeDuplicate() const {
	return new ShieldBulletPower(/*this->timeLeft*/);
}

TankPower* ShieldBulletPower::makeTankPower() const {
	return new ShieldTankPower();
}

InteractionBoolHolder ShieldBulletPower::modifiedDeathHandling(Bullet* parent) {
	return { false, true };
}

ShieldBulletPower::ShieldBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesDeathHandling = true;
}
