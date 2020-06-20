#pragma once
class BounceTankPower;

#include "tankpower.h"
#include "bouncepower.h"

class BounceTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent);
	virtual void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return BouncePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new BounceTankPower(); }
	virtual BulletPower* makeBulletPower();

	BounceTankPower();
};
