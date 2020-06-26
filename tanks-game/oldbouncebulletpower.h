#pragma once
class OldBounceBulletPower;

#include "bulletpower.h"
#include "oldbouncepower.h"

class OldBounceBulletPower : public BulletPower {
protected:
	static const short maxBounces;
	short bouncesLeft;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() {
		return OldBouncePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new OldBounceBulletPower(); } //I don't think bounces were passed on in JS
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

	virtual double getBulletSpeedMultiplier() override { return .25; }

	OldBounceBulletPower();
};
