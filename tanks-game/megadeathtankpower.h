#pragma once
class MegaDeathTankPower;

#include "tankpower.h"
#include "megadeathpower.h"

class MegaDeathTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent);
	virtual void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return MegaDeathPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new MegaDeathTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankFiringRateMultiplier() { return 4; }

	MegaDeathTankPower();
};
