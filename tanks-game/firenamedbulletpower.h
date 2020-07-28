#pragma once
class FireNamedBulletPower;

#include "bulletpower.h"
#include "firenamedpower.h"

class FireNamedBulletPower : public BulletPower {
protected:
	static const double maxBulletAcceleration;
	static const double minBulletAcceleration;
	static const double degradeAmount;
	static const double growAmount;
	double accelerationAmount;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return FireNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override;
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

	virtual double getBulletSpeedMultiplier() const override { return .5; }
	virtual double getBulletAcceleration() const override;

	FireNamedBulletPower();
	FireNamedBulletPower(double acceleration); //protected?
};
