#pragma once
class HomingTankPower;

#include "tankpower.h"
#include "homingpower.h"

class HomingTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return HomingPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new HomingTankPower(); }
	virtual BulletPower* makeBulletPower();

	HomingTankPower();
};
