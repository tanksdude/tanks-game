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

	virtual ColorValueHolder getColor() {
		return FireNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new FireNamedTankPower(); }
	virtual BulletPower* makeBulletPower();

	//bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint) override;
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	//virtual double getTankMaxSpeedMultiplier() override { return .75; }
	virtual double getTankFiringRateMultiplier() override { return .5; }
	virtual double getTankTurningIncrementMultiplier() override { return 2; }

	FireNamedTankPower();
};
