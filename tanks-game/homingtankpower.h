#pragma once
class HomingTankPower;

#include "tankpower.h"
#include "homingpower.h"

class HomingTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent);
	virtual void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return HomingPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new HomingTankPower(); }
	virtual BulletPower* makeBulletPower();

	HomingTankPower();
};
