#pragma once
class FireNamedTankPower;

#include "tankpower.h"
#include "firenamedpower.h"

class FireNamedTankPower : public TankPower {
protected:
	static const double bulletAngleDeviation;
	static const int bulletAmount;

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return FireNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new FireNamedTankPower(); }
	virtual BulletPower* makeBulletPower();

	//bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint);
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	virtual double getTankFiringRateMultiplier() { return .5; }

	FireNamedTankPower();
};
