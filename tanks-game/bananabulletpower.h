#pragma once
class BananaBulletPower;

#include "bulletpower.h"
#include "bananapower.h"

class BananaBulletPower : public BulletPower {
protected:
	static const int bananaCount;
	static const double maxNewBulletVelocity;
	static const double minNewBulletVelocity;
	//bool wasStationary; //must have velocity of zero for one tick before banana-ing

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BananaPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new BananaBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	virtual double getBulletAcceleration() const override { return -1.0/8; }

	BananaBulletPower();
};
