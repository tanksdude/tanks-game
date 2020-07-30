#include "trackingpower.h"
#include "trackingtankpower.h"
#include "trackingbulletpower.h"

TankPower* TrackingPower::makeTankPower() const {
	return new TrackingTankPower();
}

BulletPower* TrackingPower::makeBulletPower() const {
	return new TrackingBulletPower();
}

/*
HazardPower* TrackingPower::makeHazardPower() const {
	return new TrackingHazardPower();
}
*/

Power* TrackingPower::factory() {
	return new TrackingPower();
}

TrackingPower::TrackingPower() {
	return;
}
