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

	//virtual void modifiedMovement();

	//bool (*modifiedCollisionWithWall)(Tank*, Wall*);

	//virtual double getOffenseTier(); //don't want it to be a variable because a function can change its value much easier
	//virtual double getOffenseValue(); //only one tier per power
	//virtual double getDefenseTier();
	//virtual double getDefenseValue(); //return 0 by default, else 1 probably

	BounceTankPower();
};