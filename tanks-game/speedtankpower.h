#pragma once
class SpeedTankPower;

#include "tankpower.h"
#include "speedpower.h"

class SpeedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return SpeedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new SpeedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override;
	//bool tankMaxSpeedStacks = true;

	SpeedTankPower();
};
