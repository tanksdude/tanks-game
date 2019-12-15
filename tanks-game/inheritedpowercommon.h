#pragma once
#include "colorvalueholder.h"

class InheritedPowerCommon;

#include "tank.h"
#include "wall.h"
//#include "power.h"
//#include "bullet.h"

class InheritedPowerCommon {
protected:
public:
	double timeLeft;
	double maxTime;

	//ColorValueHolder color;

public:
	//virtual void initialize(Tank* parent);
	//virtual void removePower(Tank* parent);

	virtual void tick() = 0;
	virtual void powerTick() = 0;

	virtual ColorValueHolder getColor() = 0;

	virtual void modifiedMovement() { return; }
	virtual bool isDone() = 0; //as in, is finished, shouldn't exist anymore

	virtual void modifiedCollisionWithTank(Tank*) { return; } //something might use it
	virtual void modifiedCollisionWithWall(Wall*) { return; } //wallhack and bounce
	//virtual void modifiedCollisionWithPower(Power*); //probably not going to be used
	//virtual void modifiedCollisionWithBullet(Bullet*); //probably shouldn't be used
	//virtual void modifiedCollisionWithHazard(Hazard*);

	//virtual void modifiedShooting();
	//virtual void modifiedTankDrawings();

	virtual double getOffenseTier() { return 0; } //don't want it to be a variable because a function can change its value much easier
	virtual double getOffenseValue() { return 0; } //only one tier per power
	virtual double getDefenseTier() { return 0; }
	virtual double getDefenseValue() { return 0; } //return 0 by default, else 1 probably
};