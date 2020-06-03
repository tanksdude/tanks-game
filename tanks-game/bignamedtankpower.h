#pragma once
class BigNamedTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "bignamedpower.h"

class BigNamedTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return BigNamedPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier() { return .5; }
	virtual double getTankAccelerationMultiplier() { return .5; }

	virtual double getShootingMultiplier() { return 4; } //maybe *2?
	virtual double getBulletSpeedMultiplier() { return .25; }
	virtual double getBulletRadiusMultiplier() { return 4; }
	//bool bulletRadiusStacks = true;

	BigNamedTankPower();
};
