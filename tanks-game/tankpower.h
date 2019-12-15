#pragma once
class TankPower;

#include "colorvalueholder.h"
#include "tank.h"
#include "cannonpoint.h"
#include "bullet.h"
#include "wall.h"
#include "inheritedpowercommon.h"

class TankPower : public InheritedPowerCommon {
protected:
	//double timeLeft;
	//double maxTime;

public:
	virtual void initialize(Tank* parent) = 0;
	virtual void removeEffects(Tank* parent) = 0;

	virtual void tick() {
		//most shouldn't be doing anything
		return;
	}
	virtual void powerTick() {
		timeLeft--;
	}
	virtual bool isDone() {
		return timeLeft <= 0;
	}
	virtual ColorValueHolder getColor() = 0;

	virtual BulletPower* makeBulletPower() = 0;

	bool modifiesMovement = false;
	virtual void modifiedMovement(Tank*) { return; }

	bool modifiesCollisionWithEdge = false;
	virtual bool modifiedEdgeCollision(Tank*) { return false; }

	bool modifiesCollisionWithTank = false;
	virtual bool modifiedCollisionWithTank(Tank* parent, Tank* other) { return false; }
	
	bool modifiesCollisionWithWall = false;
	virtual bool modifiedCollisionWithWall(Tank*, Wall*) { return false; }

	//bool modifiesCollisionWithPower = false;
	//virtual void modifiedCollisionWithPower(Tank*, Power*) { return; } //probably not going to be used

	//bool modifiesCollisionWithBullet = false;
	//virtual void modifiedCollisionWithBullet(Tank*, Bullet*) { return; } //probably shouldn't be used

	//bool modifiesCollisionWithHazard = false;
	//virtual void modifiedCollisionWithHazard(Tank*, Hazard*) { return; }

	bool modifiesShooting = false;
	virtual void modifiedShooting(Tank* parent) { return; } //for melee-class powerups

	bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent) { return; } //for regular powerups

	bool addsShootingPoints = false;
	virtual void addShootingPoints(Tank*, std::vector<CannonPoint>*) { return; } //shouldn't need the Tank*
	
	bool modifiesTankDrawings = false;
	virtual void modifiedTankDrawings(Tank* parent) { return; }

	virtual double getShootingMultiplier() { return 1; }
	//virtual double getOffenseTier() { return 0; }
	//virtual double getOffenseValue() { return 0; }
	//virtual double getDefenseTier() { return 0; }
	//virtual double getDefenseValue() { return 0; }

};
