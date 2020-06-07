#pragma once
class BounceTankPower;

#include "tankpower.h"
#include "bouncepower.h"

class BounceTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return BouncePower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	//virtual double getTankRadiusMultiplier() { return .5; }
	virtual double getBulletSpeedMultiplier();

	BounceTankPower();
};
