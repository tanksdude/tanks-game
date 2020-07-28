#pragma once
class MegaDeathBulletPower;

#include "bulletpower.h"
#include "megadeathpower.h"

class MegaDeathBulletPower : public BulletPower {
protected: //public?
	static const double destroyWallTier; //(and destroyHazardTier)
	static const double bulletSizeMultiplierPerTick;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual void tick(Bullet* b) override; //for updating wall collision bools based on size //(override in case the Bullet* argument changes)

	virtual ColorValueHolder getColor() const override {
		return MegaDeathPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new MegaDeathBulletPower(); } //should current size be passed on?
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() const override { return 1.0/4; }
	//with bulletSizeMultiplierPerTick = 257.0/256.0, 1.0/128 or 1.0/64 might be closer to what I originally thought of
	//virtual double getBulletRadiusMultiplier() const override { return 1; }
	virtual double getBulletAcceleration() const override { return 1.0/64; }
	//with bulletSizeMultiplierPerTick = 257.0/256.0, 1.0/512 or 1.0/1024 (with a smaller radius multiplier per tick) might be closer to what I orignially thought of

	virtual double getOffenseImportance() const override { return 0; }
	virtual double getOffenseTier(Bullet*) const override;
	virtual double getDefenseImportance() const override { return 0; }
	virtual double getDefenseTier(Bullet*) const override;

	MegaDeathBulletPower();
};
