#pragma once
class GrenadeTankPower;

#include "tankpower.h"
#include "grenadepower.h"

class GrenadeTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return GrenadePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new GrenadeTankPower(); }
	virtual BulletPower* makeBulletPower();

	GrenadeTankPower();
};
