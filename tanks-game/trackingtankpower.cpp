#include "trackingtankpower.h"
#include "trackingbulletpower.h"

void TrackingTankPower::initialize(Tank* parent) {
	//nothing
}

void TrackingTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* TrackingTankPower::makeBulletPower() const {
	return new TrackingBulletPower();
}

TrackingTankPower::TrackingTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
