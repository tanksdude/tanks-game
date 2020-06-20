#pragma once
class MegaDeathBulletPower;

#include "bulletpower.h"
#include "megadeathpower.h"

class MegaDeathBulletPower : public BulletPower {
protected: //public?
	static const double destroyWallTier; //(and destroyHazardTier)
	static const double bulletSizeMultiplierPerTick;

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual void tick(Bullet* b); //for updating wall collision bools based on size

	virtual ColorValueHolder getColor() {
		return MegaDeathPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new MegaDeathBulletPower(); } //should current size be passed on?
	virtual TankPower* makeTankPower();

	//bool modifiesMovement = true;
	virtual PowerInteractionBoolHolder modifiedMovement(Bullet*);
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*);
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() { return 1.0/4; }
	//with bulletSizeMultiplierPerTick = 257.0/256.0, 1.0/128 or 1.0/64 might be closer to what I originally thought of
	//virtual double getBulletRadiusMultiplier() { return 1; }
	virtual double getBulletAcceleration() { return 1.0/64; }
	//with bulletSizeMultiplierPerTick = 257.0/256.0, 1.0/512 or 1.0/1024 (with a smaller radius multiplier per tick) might be closer to what I orignially thought of

	virtual double getOffenseImportance() { return 0; }
	virtual double getOffenseTier(Bullet*);
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier(Bullet*);

	MegaDeathBulletPower();
};
