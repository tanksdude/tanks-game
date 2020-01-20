#pragma once
class MegaDeathBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "megadeathpower.h"

class MegaDeathBulletPower : public BulletPower {
protected:

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual void powerTick(Bullet* b); //for updating wall collision bools based 

	virtual void tick() { return; }
	virtual ColorValueHolder getColor() {
		return MegaDeathPower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	//bool modifiesMovement = true;
	virtual void modifiedMovement(Bullet*);
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*);
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getOffenseImportance() { return 0; }
	//virtual double getOffenseTier();
	virtual double getOffenseTier(Bullet*);
	virtual double getDefenseImportance() { return 0; }
	//virtual double getDefenseTier();
	virtual double getDefenseTier(Bullet*);

	MegaDeathBulletPower();
};