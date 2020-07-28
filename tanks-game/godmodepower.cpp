#include "godmodepower.h"
#include "godmodetankpower.h"
#include "godmodebulletpower.h"
#include "colormixer.h"
#include "gamemanager.h"

ColorValueHolder GodmodePower::getClassColor() {
	return ColorMixer::HSVtoRGB(fmod(GameManager::getTickCount(), 360), .75, .75);
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
