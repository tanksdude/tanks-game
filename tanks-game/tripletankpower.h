#pragma once
class TripleTankPower;

#include "tankpower.h"
#include "triplepower.h"

class TripleTankPower : public TankPower {
protected:
	static const double angleDiff;

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return TriplePower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	//bool modifiesAdditionalShooting = true;
	virtual void additionalShooting(Tank* parent, CannonPoint);
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	TripleTankPower();
};
