#pragma once
class GrenadeTankPower;

#include "tankpower.h"
#include "grenadepower.h"

class GrenadeTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return GrenadePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new GrenadeTankPower(); }
	virtual BulletPower* makeBulletPower();

	GrenadeTankPower();
};
