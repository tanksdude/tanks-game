#pragma once
class MegaDeathTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "megadeathpower.h"

class MegaDeathTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return MegaDeathPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	virtual double getBulletSpeedMultiplier() { return .25; }
	//virtual double getBulletRadiusMultiplier() { return 1; }

	MegaDeathTankPower();
};