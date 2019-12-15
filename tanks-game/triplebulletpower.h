#pragma once
class TripleBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "triplepower.h"

class TripleBulletPower : public BulletPower {

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual void tick() { return; }
	virtual ColorValueHolder getColor() {
		return TriplePower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	//virtual double getOffenseTier() { return 0; }
	//virtual double getOffenseValue() { return 0; }
	//virtual double getDefenseTier() { return 0; }
	//virtual double getDefenseValue() { return 0; }

	TripleBulletPower();
};