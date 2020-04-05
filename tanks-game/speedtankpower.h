#pragma once
class SpeedTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "speedpower.h"

class SpeedTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return SpeedPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier();
	//bool tankMaxSpeedStacks = true;

	SpeedTankPower();
};