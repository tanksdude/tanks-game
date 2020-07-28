#include "speedpower.h"
#include "speedtankpower.h"
#include "speedbulletpower.h"

TankPower* SpeedPower::makeTankPower() const {
	return new SpeedTankPower();
}

BulletPower* SpeedPower::makeBulletPower() const {
	return new SpeedBulletPower();
}

/*
HazardPower* SpeedPower::makeHazardPower() const {
	return new SpeedHazardPower();
}
*/

Power* SpeedPower::factory() {
	return new SpeedPower();
}

SpeedPower::SpeedPower() {
	//does nothing
}
