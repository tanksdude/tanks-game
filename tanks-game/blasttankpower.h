#pragma once
class BlastTankPower;

#include "tankpower.h"
#include "blastpower.h"

class BlastTankPower : public TankPower {
protected:
	static const double bulletAngleDeviation;
	static const int bulletAmount;

public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return BlastPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new BlastTankPower(); }
	virtual BulletPower* makeBulletPower();

	//bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint) override;
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	virtual double getTankMaxSpeedMultiplier() override { return .5; }
	virtual double getTankAccelerationMultiplier() override { return .5; }

	BlastTankPower();
};
