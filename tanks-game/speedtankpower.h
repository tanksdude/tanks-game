#pragma once
class SpeedTankPower;

#include "tankpower.h"
#include "speedpower.h"

class SpeedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent);
	virtual void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return SpeedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new SpeedTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier();
	//bool tankMaxSpeedStacks = true;

	SpeedTankPower();
};
