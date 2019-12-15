#pragma once
class WallhackTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "wallhackpower.h"

class WallhackTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return WallhackPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	//virtual void modifiedMovement();

	virtual void modifiedCollisionWithWall(Wall*);

	//virtual void modifiedShooting(Tank* parent);
	//virtual void modifiedTankDrawings(Tank* parent);

	//virtual double getOffenseTier(); //don't want it to be a variable because a function can change its value much easier
	//virtual double getOffenseValue(); //only one tier per power
	//virtual double getDefenseTier();
	//virtual double getDefenseValue(); //return 0 by default, else 1 probably

	WallhackTankPower();
};