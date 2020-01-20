#pragma once
class HomingTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "homingpower.h"

class HomingTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return HomingPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	HomingTankPower();
};