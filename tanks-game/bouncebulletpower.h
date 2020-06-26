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

	virtual ColorValueHolder getColor() {
		return BouncePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate();
	virtual TankPower* makeTankPower();
	
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

	virtual double getBulletSpeedMultiplier() override { return .5; }

	BounceBulletPower();
	BounceBulletPower(short bounces);
};
