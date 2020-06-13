#pragma once
class BlastTankPower;

#include "tankpower.h"
#include "blastpower.h"

class BlastTankPower : public TankPower {
protected:
	static const double bulletAngleDeviation;
	static const int bulletAmount;

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return BlastPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	//bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint);
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	virtual double getTankMaxSpeedMultiplier() { return .5; }
	virtual double getTankAccelerationMultiplier() { return .5; }

	BlastTankPower();
};
