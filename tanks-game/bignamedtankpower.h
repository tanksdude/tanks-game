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

	virtual double getBulletSpeedMultiplier() { return .125; }
	virtual double getBulletRadiusMultiplier() { return 4; }
	//bool bulletRadiusStacks = true;

	virtual double getShootingMultiplier() { return 2; } // *4 is JS setting

	BigNamedTankPower();
};