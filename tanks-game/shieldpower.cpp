#include "shieldpower.h"

const double ShieldPower::barrierStrength = 50; //TODO: this good?

//process of narrowing down the specific color:
/*
//static ColorValueHolder getClassColor() { return ColorValueHolder(0.625f, 0.75f, 0.9375f); } //very light blue (baby blue?)
//static ColorValueHolder getClassColor() { return ColorValueHolder(0.0625f, 0.375f, 0.6875f); } //sapphire
//static ColorValueHolder getClassColor() { return ColorValueHolder(47/255.0f, 117/255.0f, 187/255.0f); } //kinda sapphire
//static ColorValueHolder getClassColor() { return ColorValueHolder(100/255.0f, 149/255.0f, 222/255.0f); } //kinda sapphire, again
//static ColorValueHolder getClassColor() { return ColorValueHolder(116/255.0f, 161/255.0f, 229/255.0f); } //less blue (good, but now I've decided on green)
//static ColorValueHolder getClassColor() { return ColorValueHolder(161/255.0f, 229/255.0f, 116/255.0f); } //green equivalent
//static ColorValueHolder getClassColor() { return ColorValueHolder(116/255.0f, 229/255.0f, 127/255.0f); } //lil' more cyan
//static ColorValueHolder getClassColor() { return ColorValueHolder(0xA7/255.0f, 0xE0/255.0f, 0xA3/255.0f); } //new approach, more gray
static ColorValueHolder getClassColor() { return ColorValueHolder(0xB6/255.0f, 0xE6/255.0f, 0x9D/255.0f); } //less gray, shift towards yellow (good)
//static ColorValueHolder getClassColor() { return ColorValueHolder(0xB3/255.0f, 0xF3/255.0f, 0x90/255.0f); } //less gray
//static ColorValueHolder getClassColor() { return ColorValueHolder(0x94/255.0f, 0xEE/255.0f, 0x63/255.0f); } //turn down light
//static ColorValueHolder getClassColor() { return ColorValueHolder(0x9A/255.0f, 0xD8/255.0f, 0x79/255.0f); } //more gray
//static ColorValueHolder getClassColor() { return ColorValueHolder(0x80/255.0f, 0xDD/255.0f, 0x80/255.0f); } //restart: life but *really* white
//static ColorValueHolder getClassColor() { return ColorValueHolder(0x98/255.0f, 0xC8/255.0f, 0x80/255.0f); } //less green, shift yellow
*/

TankPower* ShieldPower::makeTankPower() const {
	return new ShieldTankPower();
}

BulletPower* ShieldPower::makeBulletPower() const {
	return new ShieldBulletPower();
}

/*
HazardPower* ShieldPower::makeHazardPower() const {
	return new ShieldHazardPower();
}
*/

Power* ShieldPower::factory() {
	return new ShieldPower();
}

ShieldPower::ShieldPower() {
	return;
}



void ShieldTankPower::initialize(Tank* parent) {
	//nothing
}

void ShieldTankPower::removeEffects(Tank* parent) {
	//nothing
}

TankPower* ShieldTankPower::makeDuplicate() const {
	return new ShieldTankPower(this->timeLeft);
}

BulletPower* ShieldTankPower::makeBulletPower() const {
	return new ShieldBulletPower();
}

InteractionBoolHolder ShieldTankPower::modifiedDeathHandling(Tank* parent) {
	if (this->timeLeft < ShieldPower::barrierStrength) {
		return { false, true };
	}
	this->timeLeft -= ShieldPower::barrierStrength;
	return { false, false };
}

ShieldTankPower::ShieldTankPower() : ShieldTankPower(500) {}

ShieldTankPower::ShieldTankPower(double life) {
	maxTime = life;
	timeLeft = 500;

	modifiesDeathHandling = true;
}



void ShieldBulletPower::initialize(Bullet* parent) {
	//nothing
}

void ShieldBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

BulletPower* ShieldBulletPower::makeDuplicate() const {
	return new ShieldBulletPower(/*this->timeLeft*/);
}

TankPower* ShieldBulletPower::makeTankPower() const {
	return new ShieldTankPower();
}

InteractionBoolHolder ShieldBulletPower::modifiedDeathHandling(Bullet* parent) {
	//TODO: should this be the case?
	return { false, true };
}

ShieldBulletPower::ShieldBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesDeathHandling = true;
}
