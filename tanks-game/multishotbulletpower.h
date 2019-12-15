#pragma once
#include "power.h"
#include "bulletpower.h"
#include "multishotpower.h"

class MultishotBulletPower : public BulletPower {

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual void tick() { return; }
	virtual ColorValueHolder getColor() {
		return MultishotPower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	//virtual double getOffenseTier() { return 0; }
	//virtual double getOffenseValue() { return 0; }
	//virtual double getDefenseTier() { return 0; }
	//virtual double getDefenseValue() { return 0; }

	MultishotBulletPower();
};