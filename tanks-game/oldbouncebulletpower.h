#pragma once
class OldBounceBulletPower;

#include "bulletpower.h"
#include "oldbouncepower.h"

class OldBounceBulletPower : public BulletPower {
protected:
public:
	static const short maxBounces;
	short bouncesLeft;

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return OldBouncePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new OldBounceBulletPower(); }
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

	virtual double getBulletSpeedMultiplier() { return .25; }

	OldBounceBulletPower();
};
