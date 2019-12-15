#pragma once
class BounceTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "bouncepower.h"

class BounceTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return BouncePower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	virtual double getBulletSpeedMultiplier();
	//virtual double getBulletRadiusMultiplier();

	//virtual double getOffenseTier();
	//virtual double getOffenseValue();
	//virtual double getDefenseTier();
	//virtual double getDefenseValue();

	BounceTankPower();
};