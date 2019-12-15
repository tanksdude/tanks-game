#pragma once
class TankPower;

#include "colorvalueholder.h"
#include "tank.h"
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

	virtual void modifiedMovement() { return; }

	//virtual void modifiedCollisionWithTank(Tank*); //TODO: replace with std::function
	//virtual void modifiedCollisionWithWall(Wall*);
	//virtual void modifiedCollisionWithPower(Power*); //probably not going to be used
	//virtual void modifiedCollisionWithBullet(Bullet*); //probably shouldn't be used
	//virtual void modifiedCollisionWithHazard(Hazard*);

	virtual void modifiedShooting(Tank* parent) { return; }
	virtual void modifiedTankDrawings(Tank* parent) { return; }

	//virtual double getOffenseTier() { return 0; } //don't want it to be a variable because a function can change its value much easier
	//virtual double getOffenseValue() { return 0; } //only one tier per power
	//virtual double getDefenseTier() { return 0; }
	//virtual double getDefenseValue() { return 0; } //return 0 by default, else 1 probably
};
