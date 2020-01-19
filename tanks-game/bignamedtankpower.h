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

	//virtual double getOffenseImportance() { return 0; }
	//virtual double getOffenseTier() { return 0; }
	//virtual double getDefenseImportance() { return 0; }
	//virtual double getDefenseTier() { return 0; }

	virtual double getBulletSpeedMultiplier() { return .25; }
	virtual double getBulletRadiusMultiplier() { return 4; }

	BigNamedTankPower();
};