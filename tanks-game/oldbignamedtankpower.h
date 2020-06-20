#pragma once
class OldBigNamedTankPower;

#include "tankpower.h"
#include "oldbignamedpower.h"

class OldBigNamedTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return OldBigNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new OldBigNamedTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier() { return .5; }
	virtual double getTankAccelerationMultiplier() { return .5; }
	virtual double getTankFiringRateMultiplier() { return 4; } //stacked in JS, not sure if it should stack here

	OldBigNamedTankPower();
};
