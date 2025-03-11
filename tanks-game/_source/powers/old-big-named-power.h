#pragma once
#include "big-named-power.h"

class OldBigNamedPower : public BigNamedPower {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return OldBigNamedPower::getClassName(); }
	static std::string getClassName() { return "old_big"; }
	virtual ColorValueHolder getColor() const override { return OldBigNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return BigNamedPower::getClassColor(); } //red

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	OldBigNamedPower();
	static Power* factory();
};



class OldBigNamedTankPower : public BigNamedTankPower {
public:
	virtual ColorValueHolder getColor() const override { return OldBigNamedPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new OldBigNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	OldBigNamedTankPower();
};



class OldBigNamedBulletPower : public BigNamedBulletPower {
public:
	virtual ColorValueHolder getColor() const override { return OldBigNamedPower::getClassColor(); }

	virtual BulletPower* makeDuplicate() const override { return new OldBigNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	virtual double getBulletSpeedMultiplier() const override { return .5; }
	//bool bulletSpeedStacks = true;

	OldBigNamedBulletPower();
};
