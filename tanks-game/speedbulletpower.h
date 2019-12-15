#pragma once
class SpeedBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "speedpower.h"
#include "tankpower.h"

class SpeedBulletPower : public BulletPower {

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual void tick() { return; }
	virtual ColorValueHolder getColor() {
		return SpeedPower::getClassColor();
	}

	virtual TankPower* makeTankPower();
	
	//virtual void modifiedMovement();

	//virtual void modifiedBulletDrawings(Bullet* parent); //probably not going to be used

	//virtual double getOffenseTier() { return 0; } //don't want it to be a variable because a function can change its value much easier
	//virtual double getOffenseValue() { return 0; } //only one tier per power
	//virtual double getDefenseTier() { return 0; }
	//virtual double getDefenseValue() { return 0; } //return 0 by default, else 1 probably

	SpeedBulletPower();
};