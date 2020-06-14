#pragma once
class BigNamedTankPower;

#include "tankpower.h"
#include "bignamedpower.h"

class BigNamedTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return BigNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new BigNamedTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier() { return .5; }
	virtual double getTankAccelerationMultiplier() { return .5; }
	virtual double getFiringRateMultiplier() { return 4; } //maybe *2?

	BigNamedTankPower();
};
