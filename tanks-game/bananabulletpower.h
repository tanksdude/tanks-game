#pragma once
class BananaBulletPower;

#include "bulletpower.h"
#include "bananapower.h"

class BananaBulletPower : public BulletPower {
protected:
	static const int bananaCount;
	static const double maxNewBulletVelocity;
	static const double minNewBulletVelocity;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() {
		return BananaPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new BananaBulletPower(); }
	virtual TankPower* makeTankPower();

	//bool modifiesMovement = true;
	virtual PowerInteractionBoolHolder modifiedMovement(Bullet*) override;
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	virtual double getBulletAcceleration() override { return -1.0/8; }

	BananaBulletPower();
};
