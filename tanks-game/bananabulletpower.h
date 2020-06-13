#pragma once
class BananaBulletPower;

#include "bulletpower.h"
#include "bananapower.h"

class BananaBulletPower : public BulletPower {
protected:
	static const int bananaCount;

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return BananaPower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	//bool modifiesMovement = true;
	virtual void modifiedMovement(Bullet*);
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	virtual double getBulletAcceleration() { return -1.0/8; }

	BananaBulletPower();
};
