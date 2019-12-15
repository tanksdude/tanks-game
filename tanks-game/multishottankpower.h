#pragma once
class MultishotTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "multishotpower.h"
#include "cannonpoint.h"

class MultishotTankPower : public TankPower {
public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return MultishotPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	//bool addsShootingPoints = true;
	virtual void addShootingPoints(Tank*, std::vector<CannonPoint>*);

	virtual double getShootingMultiplier() { return .5; }
	//virtual double getOffenseTier(); //don't want it to be a variable because a function can change its value much easier
	//virtual double getOffenseValue(); //only one tier per power
	//virtual double getDefenseTier();
	//virtual double getDefenseValue(); //return 0 by default, else 1 probably

	MultishotTankPower();
};