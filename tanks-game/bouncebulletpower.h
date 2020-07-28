#pragma once
class BounceBulletPower;

#include "bulletpower.h"
#include "bouncepower.h"

class BounceBulletPower : public BulletPower {
protected:
	static const short maxBounces;
	short bouncesLeft;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BouncePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override;
	virtual TankPower* makeTankPower() const override;
	
	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithEdge = true;
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) override;
	//bool overridesEdgeCollision = true;
	//bool modifiedEdgeCollisionCanWorkWithOthers = false;
	//bool modifiedEdgeCollisionCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() const override { return .5; }

	BounceBulletPower();
	BounceBulletPower(short bounces);
};
