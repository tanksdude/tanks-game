#pragma once
#include "power.h"
#include "tankpower.h"

class SpeedTankPower : public TankPower {

public:
	virtual void initialize(Tank*);
	virtual void removePower(Tank*);

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