#pragma once
class OldBigNamedBulletPower;

#include "bulletpower.h"
#include "oldbignamedpower.h"
#include "constants.h"

class OldBigNamedBulletPower : public BulletPower {
protected:

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return OldBigNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new OldBigNamedBulletPower(); }
	virtual TankPower* makeTankPower();

	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*);
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() { return .5; }
	//bool bulletSpeedStacks = true;
	virtual double getBulletRadiusMultiplier() { return 4; }
	//bool bulletRadiusStacks = true;

	virtual double getOffenseImportance() { return 0; }
	virtual double getOffenseTier(Bullet*) { return DESTRUCTION_TIER; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier(Bullet*) { return DESTRUCTION_TIER; }

	OldBigNamedBulletPower();
};
