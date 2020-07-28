#pragma once
class GrenadeBulletPower;

#include "bulletpower.h"
#include "grenadepower.h"

class GrenadeBulletPower : public BulletPower {
protected:
	static const double degradeAmount;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return GrenadePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new GrenadeBulletPower(); } //should current size be passed on?
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletAcceleration() const override { return -1.0/16; }

	GrenadeBulletPower();
};
