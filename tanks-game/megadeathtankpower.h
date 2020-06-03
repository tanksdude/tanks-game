#pragma once
class MegaDeathTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "megadeathpower.h"

class MegaDeathTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return MegaDeathPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	virtual double getShootingMultiplier() { return 4; }
	virtual double getBulletSpeedMultiplier() { return 1.0/4; }
	//with bulletSizeMultiplierPerTick = 257.0/256.0, 1.0/128 or 1.0/64 might be closer to what I originally thought of
	//virtual double getBulletRadiusMultiplier() { return 1; }
	virtual double getBulletAcceleration() { return 1.0/64; }
	//with bulletSizeMultiplierPerTick = 257.0/256.0, 1.0/512 or 1.0/1024 (with a smaller radius multiplier per tick) might be closer to what I orignially thought of

	MegaDeathTankPower();
};
