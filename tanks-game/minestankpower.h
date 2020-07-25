#pragma once
class MinesTankPower;

#include "tankpower.h"
#include "minespower.h"

class MinesTankPower : public TankPower {
protected:
	static const double bulletDistance; //percentage from center (beginning of cannon) to end of cannon

public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual void tick(Tank* t) override; //for updating modifiesAdditionalShooting based on whether there are any other powers that modify additionalShooting
	//might need a secondary tick for this; one tick to do stuff, another tick to update interaction bools

	virtual ColorValueHolder getColor() {
		return MinesPower::getClassColor();
	}
	virtual double getColorImportance() override {
		return MinesPower::getClassColorImportance();
	}

	virtual TankPower* makeDuplicate() { return new MinesTankPower(); }
	virtual BulletPower* makeBulletPower();

	//bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint) override;
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	virtual double getTankFiringRateMultiplier() override { return .25; } //.5?
	
	MinesTankPower();
};
