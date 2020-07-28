#pragma once
class BigNamedTankPower;

#include "tankpower.h"
#include "bignamedpower.h"

class BigNamedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BigNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new BigNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override { return .5; }
	virtual double getTankAccelerationMultiplier() const override { return .5; }
	virtual double getTankFiringRateMultiplier() const override { return 4; } //maybe *2?

	BigNamedTankPower();
};
