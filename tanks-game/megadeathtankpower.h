#pragma once
class MegaDeathTankPower;

#include "tankpower.h"
#include "megadeathpower.h"

class MegaDeathTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return MegaDeathPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new MegaDeathTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getFiringRateMultiplier() { return 4; }

	MegaDeathTankPower();
};
