#pragma once
class SpeedTankPower;

#include "tankpower.h"
#include "speedpower.h"

class SpeedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return SpeedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new SpeedTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier() override;
	//bool tankMaxSpeedStacks = true;

	SpeedTankPower();
};
