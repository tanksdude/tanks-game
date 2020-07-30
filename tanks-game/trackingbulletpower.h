#pragma once
class TrackingBulletPower;

#include "bulletpower.h"
#include "trackingpower.h"

class TrackingBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return TrackingPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new TrackingBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = false;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;
	
	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true; //false means also use the default
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() const override { return .25; }

	TrackingBulletPower();
};
