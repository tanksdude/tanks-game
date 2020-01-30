#pragma once
class FireNamedTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "firenamedpower.h"

class FireNamedTankPower : public TankPower {
protected:
	static const double bulletAngleDeviation;
	static const double maxBulletAcceleration;
	static const double minBulletAcceleration;
	static const int bulletAmount;

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return FireNamedPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	//bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint);
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	//virtual double getBulletAcceleration() { return -1.0/16; }

	FireNamedTankPower();
};