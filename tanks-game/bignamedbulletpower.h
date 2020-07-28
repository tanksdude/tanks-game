#pragma once
class BigNamedBulletPower;

#include "bulletpower.h"
#include "bignamedpower.h"
#include "constants.h"

class BigNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BigNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new BigNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() const override { return .25; }
	virtual double getBulletRadiusMultiplier() const override { return 4; }
	//bool bulletRadiusStacks = true;

	virtual double getOffenseImportance() const override { return 0; }
	virtual double getOffenseTier(Bullet*) const override { return DESTRUCTION_TIER; }
	virtual double getDefenseImportance() const override { return 0; }
	virtual double getDefenseTier(Bullet*) const override { return DESTRUCTION_TIER; }

	BigNamedBulletPower();
};
