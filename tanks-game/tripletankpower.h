#pragma once
class TripleTankPower;

#include "tankpower.h"
#include "triplepower.h"

class TripleTankPower : public TankPower {
protected:
	static const double angleDiff;

public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return TriplePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new TripleTankPower(); }
	virtual BulletPower* makeBulletPower();

	//bool modifiesAdditionalShooting = true;
	virtual void additionalShooting(Tank* parent, CannonPoint) override;
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	TripleTankPower();
};
