#pragma once
class BulletPower;

#include "colorvalueholder.h"
#include "tank.h"
#include "bullet.h"
#include "wall.h"
#include "inheritedpowercommon.h"
#include "tankpower.h"

typedef bool (BulletPower::*memberFuncPointerForBulletPower)(Bullet*, Wall*);

class BulletPower : public InheritedPowerCommon{
	friend class PowerFunctionHelper;
protected:
public:
	//double timeLeft = 0;
	//double maxTime = -1; //bullet powers typically last forever, so they should default to -1

public:
	virtual void initialize(Bullet* parent) = 0; //unlikely to be used
	virtual void removeEffects(Bullet* parent) = 0; //not really needed

	virtual void tick() = 0; //most will be doing a lot, though they shouldn't need this, but just in case
	virtual void powerTick() {
		return;
	}
	virtual bool isDone() {
		return false;
	}
	virtual ColorValueHolder getColor() = 0;

	virtual TankPower* makeTankPower() = 0;

	bool modifiesMovement = false;
	virtual void modifiedMovement(Bullet*) { return; }

	bool modifiesCollisionWithEdge = false;
	virtual bool modifiedEdgeCollision(Bullet*) { return false; }

	bool modifiesCollisionWithTank = false;
	virtual bool modifiedCollisionWithTank(Bullet*, Tank*) { return false; }

	bool modifiesCollisionWithWall = false;
	virtual bool modifiedCollisionWithWall(Bullet*, Wall*) { return false; }

	//bool modifiesCollisionWithPower = false;
	//virtual void modifiedCollisionWithPower(Bullet*, Power*) { return; } //probably shouldn't be used

	//bool modifiesCollisionWithBullet = false;
	//virtual void modifiedCollisionWithBullet(Bullet* parent, Bullet* other) { return; } //probably shouldn't be used

	//bool modifiesCollisionWithHazard = false;
	//virtual void modifiedCollisionWithHazard(Bullet*, Hazard*);

	bool modifiesBulletDrawings = false;
	virtual void modifiedBulletDrawings(Bullet* parent) { return; } //probably not going to be used

	//virtual double getOffenseTier() { return 0; }
	//virtual double getOffenseValue() { return 0; }
	//virtual double getDefenseTier() { return 0; }
	//virtual double getDefenseValue() { return 0; }
};