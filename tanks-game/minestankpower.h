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

	virtual ColorValueHolder getColor() {
		return MinesPower::getClassColor();
	}
	virtual double getColorImportance() override {
		return MinesPower::getClassColorImportance();
	}

	virtual TankPower* makeDuplicate() { return new MinesTankPower(); }
	virtual BulletPower* makeBulletPower();

	//bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint);
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	virtual double getTankFiringRateMultiplier() override { return .25; } //.5?
	
	MinesTankPower();
};
