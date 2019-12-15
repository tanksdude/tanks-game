#pragma once
class BulletPower;

#include "colorvalueholder.h"
#include "tank.h"
#include "bullet.h"
#include "wall.h"
#include "inheritedpowercommon.h"
#include "tankpower.h"

class BulletPower : public InheritedPowerCommon{
protected:
	double timeLeft = 0;
	double maxTime = -1; //bullet powers typically last forever, so they should default to -1

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

	virtual void modifiedMovement() { return; }

	virtual void modifiedCollisionWithTank(Tank*) { return; } //TODO: replace with std::function
	virtual void modifiedCollisionWithWall(Wall*) { return; }
	//virtual void modifiedCollisionWithPower(Power*); //probably shouldn't be used
	//virtual void modifiedCollisionWithBullet(Bullet*); //probably shouldn't be used
	//virtual void modifiedCollisionWithHazard(Hazard*);

	virtual void modifiedBulletDrawings(Bullet* parent) { return; } //probably not going to be used

	//virtual double getOffenseTier() { return 0; } //don't want it to be a variable because a function can change its value much easier
	//virtual double getOffenseValue() { return 0; } //only one tier per power
	//virtual double getDefenseTier() { return 0; }
	//virtual double getDefenseValue() { return 0; } //return 0 by default, else 1 probably
};