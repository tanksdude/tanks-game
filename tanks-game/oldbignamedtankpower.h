#pragma once
class OldBigNamedTankPower;

#include "tankpower.h"
#include "oldbignamedpower.h"

class OldBigNamedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return OldBigNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new OldBigNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override { return .5; }
	virtual double getTankAccelerationMultiplier() const override { return .5; }
	virtual double getTankFiringRateMultiplier() const override { return 4; } //stacked in JS, not sure if it should stack here

	OldBigNamedTankPower();
};
