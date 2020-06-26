#include "speedpower.h"
#include "speedtankpower.h"
#include "speedbulletpower.h"

TankPower* SpeedPower::makeTankPower() {
	return new SpeedTankPower();
}

BulletPower* SpeedPower::makeBulletPower() {
	return new SpeedBulletPower();
}

/*
HazardPower* SpeedPower::makeHazardPower(){
	return new SpeedHazardPower();
}
*/

Power* SpeedPower::factory() {
	return new SpeedPower();
}

SpeedPower::SpeedPower() {
	//does nothing
}
