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

	virtual BulletPower* makeBulletPower();

	virtual double getBulletAcceleration() { return -1.0/16; }

	GrenadeTankPower();
};
