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

	//virtual void modifiedMovement();

	//void (*modifiedCollisionWithWall)(Bullet*, Wall*);

	//virtual double getOffenseTier() { return 0; } //don't want it to be a variable because a function can change its value much easier
	//virtual double getOffenseValue() { return 0; } //only one tier per power
	//virtual double getDefenseTier() { return 0; }
	//virtual double getDefenseValue() { return 0; } //return 0 by default, else 1 probably

	MultishotBulletPower();
};