#pragma once
class TankPower;

#include "tank.h"

class TankPower {
protected:
	double timeLeft;
	double maxTime;

public:
	virtual void initialize(Tank* parent);
	virtual void removePower(Tank* parent);

	virtual void tick();

	virtual void modifiedMovement();

	virtual void modifiedCollisionWithTank();
	virtual void modifiedCollisionWithWall();
	virtual void modifiedCollisionWithPower(); //probably not going to be used
	virtual void modifiedCollisionWithBullet(); //probably shouldn't be used
	//virtual void modifiedCollisionWithHazard();

	virtual void modifiedShooting();
	virtual void modifiedTankDrawings();

	virtual double getOffenseTier(); //don't want it to be a variable because a function can change its value much easier
	virtual double getOffenseValue(); //only one tier per power
	virtual double getDefenseTier();
	virtual double getDefenseValue(); //return 0 by default, else 1 probably
};
