#pragma once
class BigNamedBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "bignamedpower.h"
#include "constants.h"

class BigNamedBulletPower : public BulletPower {
protected:

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual void tick() { return; }
	virtual ColorValueHolder getColor() {
		return BigNamedPower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*);
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithCircleHazard = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithCircleHazard(Bullet*, CircleHazard*);
	//bool overridesCollisionWithCircleHazard = true;
	//bool modifiedCollisionWithCircleHazardCanWorkWithOthers = true;
	//bool modifiedCollisionWithCircleHazardCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithRectHazard = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithRectHazard(Bullet*, RectHazard*);
	//bool overridesCollisionWithRectHazard = true;
	//bool modifiedCollisionWithRectHazardCanWorkWithOthers = true;
	//bool modifiedCollisionWithRectHazardCanOnlyWorkIndividually = false;

	virtual double getOffenseImportance() { return 0; }
	virtual double getOffenseTier(Bullet*) { return DESTRUCTION_TIER; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier(Bullet*) { return DESTRUCTION_TIER; }

	BigNamedBulletPower();
};