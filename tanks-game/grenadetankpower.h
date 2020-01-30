#pragma once
class GrenadeTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
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