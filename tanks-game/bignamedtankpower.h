#pragma once
class BigNamedTankPower;

#include "tankpower.h"
#include "bignamedpower.h"

class BigNamedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return BigNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new BigNamedTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier() override { return .5; }
	virtual double getTankAccelerationMultiplier() override { return .5; }
	virtual double getTankFiringRateMultiplier() override { return 4; } //maybe *2?

	BigNamedTankPower();
};
