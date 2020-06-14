#pragma once
class HomingTankPower;

#include "tankpower.h"
#include "homingpower.h"

class HomingTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return HomingPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new HomingTankPower(); }
	virtual BulletPower* makeBulletPower();

	HomingTankPower();
};
