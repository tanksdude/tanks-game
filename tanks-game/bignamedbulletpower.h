#pragma once
class BigNamedBulletPower;

#include "bulletpower.h"
#include "bignamedpower.h"
#include "constants.h"

class BigNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return BigNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new BigNamedBulletPower(); }
	virtual TankPower* makeTankPower();

	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*);
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() { return .25; }
	virtual double getBulletRadiusMultiplier() { return 4; }
	//bool bulletRadiusStacks = true;

	virtual double getOffenseImportance() { return 0; }
	virtual double getOffenseTier(Bullet*) { return DESTRUCTION_TIER; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier(Bullet*) { return DESTRUCTION_TIER; }

	BigNamedBulletPower();
};
