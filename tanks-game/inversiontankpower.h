#pragma once
class InversionTankPower;

#include "tankpower.h"
#include "inversionpower.h"

class InversionTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return InversionPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new InversionTankPower(); }
	virtual BulletPower* makeBulletPower();

	//probably shouldn't use this:
	//bool modifiesMovement = true;
	//virtual PowerInteractionBoolHolder modifiedMovement(Tank*);
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	//need something like this:
	//virtual double getTankTurningMultiplier() { return -1; }

	InversionTankPower();
};
