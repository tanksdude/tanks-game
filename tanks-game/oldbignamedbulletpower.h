#pragma once
class OldBigNamedBulletPower;

#include "bulletpower.h"
#include "oldbignamedpower.h"
#include "constants.h"

class OldBigNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return OldBigNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new OldBigNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() const override { return .5; }
	//bool bulletSpeedStacks = true;
	virtual double getBulletRadiusMultiplier() const override { return 4; }
	//bool bulletRadiusStacks = true;

	virtual double getOffenseImportance() const override { return 0; }
	virtual double getOffenseTier(Bullet*) const override { return DESTRUCTION_TIER; }
	virtual double getDefenseImportance() const override { return 0; }
	virtual double getDefenseTier(Bullet*) const override { return DESTRUCTION_TIER; }

	OldBigNamedBulletPower();
};
