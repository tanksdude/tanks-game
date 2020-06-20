#pragma once
class BounceBulletPower;

#include "bulletpower.h"
#include "bouncepower.h"

class BounceBulletPower : public BulletPower {
protected:
	static const short maxBounces;
	short bouncesLeft;

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return BouncePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate();
	virtual TankPower* makeTankPower();
	
	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*);
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithEdge = true;
	virtual PowerInteractionBoolHolder modifiedEdgeCollision(Bullet*);
	//bool overridesEdgeCollision = true;
	//bool modifiedEdgeCollisionCanWorkWithOthers = false;
	//bool modifiedEdgeCollisionCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() { return .5; }

	BounceBulletPower();
	BounceBulletPower(short bounces);
};
