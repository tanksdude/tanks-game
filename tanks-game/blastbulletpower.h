#pragma once
class BlastBulletPower;

#include "bulletpower.h"
#include "blastpower.h"

class BlastBulletPower : public BulletPower {
protected:
	static const double maxBulletAcceleration;
	static const double minBulletAcceleration;
	static const double degradeAmount;
	double accelerationAmount;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BlastPower::getClassColor();
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

	virtual double getBulletRadiusMultiplier() const override { return .25; }
	virtual double getBulletAcceleration() const override;

	BlastBulletPower();
	BlastBulletPower(double acceleration); //protected?
};
