#pragma once
class FireNamedTankPower;

#include "tankpower.h"
#include "firenamedpower.h"

class FireNamedTankPower : public TankPower {
protected:
	static const double bulletAngleDeviation;
	static const int bulletAmount;

public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return FireNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new FireNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint) override;
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	//virtual double getTankMaxSpeedMultiplier() const override { return .75; }
	virtual double getTankFiringRateMultiplier() const override { return .5; }
	virtual double getTankTurningIncrementMultiplier() const override { return 2; }

	FireNamedTankPower();
};
