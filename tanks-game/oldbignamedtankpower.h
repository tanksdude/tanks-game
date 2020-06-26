#pragma once
class OldBigNamedTankPower;

#include "tankpower.h"
#include "oldbignamedpower.h"

class OldBigNamedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return OldBigNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new OldBigNamedTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier() override { return .5; }
	virtual double getTankAccelerationMultiplier() override { return .5; }
	virtual double getTankFiringRateMultiplier() override { return 4; } //stacked in JS, not sure if it should stack here

	OldBigNamedTankPower();
};
