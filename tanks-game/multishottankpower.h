#pragma once
class MultishotTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "multishotpower.h"
#include "cannonpoint.h"

class MultishotTankPower : public TankPower {
public: //protected?
	static void addCannonPointsDummy(Tank*, std::vector<CannonPoint>*);

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return MultishotPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	//virtual void modifiedMovement();

	//bool (*modifiedCollisionWithWall)(Tank*, Wall*);
	//void (*addShootingPoints)(Tank*, std::vector<CannonPoint>*);

	//virtual double getOffenseTier(); //don't want it to be a variable because a function can change its value much easier
	//virtual double getOffenseValue(); //only one tier per power
	//virtual double getDefenseTier();
	//virtual double getDefenseValue(); //return 0 by default, else 1 probably

	MultishotTankPower();
};