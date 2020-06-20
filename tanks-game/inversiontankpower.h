#pragma once
class InversionTankPower;

#include "tankpower.h"
#include "inversionpower.h"

class InversionTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent);
	virtual void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return InversionPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new InversionTankPower(); }
	virtual BulletPower* makeBulletPower();

	//maybe use this?:
	//bool modifiesMovement = true;
	//virtual PowerInteractionBoolHolder modifiedMovement(Tank*);
	//bool overridesMovement = true;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	virtual double getTankTurningIncrementMultiplier() { return -1; }
	//bool tankTurningIncrementStacks = true;

	InversionTankPower();
};
