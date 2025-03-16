#pragma once
#include "../power.h"

class SpeedPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return SpeedPower::getClassName(); }
	static std::string getClassName() { return "speed"; }
	virtual ColorValueHolder getColor() const override { return SpeedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.25f, 0.25f, 1.0f); } //blue //JS: #4444FF

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	SpeedPower();
	static Power* factory();
};



class SpeedTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return SpeedPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new SpeedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override { return 2; }
	//bool tankMaxSpeedStacks = true;

	SpeedTankPower();
};



class SpeedBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override { return SpeedPower::getClassColor(); }
	virtual std::string getColorIdentifier() const override { return SpeedPower::getClassName(); }

	virtual BulletPower* makeDuplicate() const override { return new SpeedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	SpeedBulletPower();
};
