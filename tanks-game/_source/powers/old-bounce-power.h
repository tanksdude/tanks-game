#pragma once
#include "bounce-power.h"

class OldBouncePower : public BouncePower {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return OldBouncePower::getClassName(); }
	static std::string getClassName() { return "old_bounce"; }
	virtual ColorValueHolder getColor() const override { return OldBouncePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return BouncePower::getClassColor(); } //pink

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	OldBouncePower();
	static Power* factory();
};



class OldBounceTankPower : public BounceTankPower {
public:
	virtual ColorValueHolder getColor() const override { return OldBouncePower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new OldBounceTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankAccelerationMultiplier() const override { return .5; }
	virtual double getTankRadiusMultiplier() const override { return .5; } //JS: stacks (as did everything)
	virtual double getTankFiringRateMultiplier() const override { return .5; }

	OldBounceTankPower();
};



class OldBounceBulletPower : public BounceBulletPower {
public:
	virtual ColorValueHolder getColor() const override { return OldBouncePower::getClassColor(); }
	virtual std::string getColorIdentifier() const override { return OldBouncePower::getClassName(); }

	virtual BulletPower* makeDuplicate() const override { return new OldBounceBulletPower(); } //not sure if bounces were passed on in JS
	virtual TankPower* makeTankPower() const override;

	virtual double getBulletSpeedMultiplier() const override { return .25; }

	OldBounceBulletPower();
};
