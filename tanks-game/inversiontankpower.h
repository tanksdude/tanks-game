#pragma once
class InversionTankPower;

#include "tankpower.h"
#include "inversionpower.h"

class InversionTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return InversionPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new InversionTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//maybe use this?:
	//bool modifiesMovement = true;
	//virtual InteractionBoolHolder modifiedMovement(Tank*) override;
	//bool overridesMovement = true;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	virtual double getTankTurningIncrementMultiplier() const override { return -1; }
	//bool tankTurningIncrementStacks = true;

	InversionTankPower();
};
