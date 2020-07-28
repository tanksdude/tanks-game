#pragma once
class ShotgunTankPower;

#include "tankpower.h"
#include "shotgunpower.h"

class ShotgunTankPower : public TankPower {
protected:
	static const int bulletSpreadCount; //number of extra bullets, *2 (shotgun shoots four extra so this number is 2)

public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return ShotgunPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new ShotgunTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesAdditionalShooting = true;
	virtual void additionalShooting(Tank* parent, CannonPoint) override;
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	//virtual double getTankFiringRateMultiplier() const override { return .5; }

	ShotgunTankPower();
};
