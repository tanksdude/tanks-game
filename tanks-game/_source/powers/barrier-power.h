#pragma once
#include "../power.h"

class BarrierPower : public Power {
	//called life in JS Tanks
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "old", "random-old", "random" };
		//no supermix; it'd be a little too strong
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return BarrierPower::getClassName(); }
	static std::string getClassName() { return "barrier"; }
	virtual ColorValueHolder getColor() const override { return BarrierPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.0f, 0.5f, 0.25f); } //dark green //JS: #008844

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	BarrierPower();
	static Power* factory();
};



class BarrierTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return BarrierPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new BarrierTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesDeathHandling = true;
	virtual InteractionBoolHolder modifiedDeathHandling(const Tank* parent) override;

	BarrierTankPower();
};



class BarrierBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override { return BarrierPower::getClassColor(); }
	virtual std::string getColorIdentifier() const override { return BarrierPower::getClassName(); }

	virtual BulletPower* makeDuplicate() const override { return new BarrierBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesDeathHandling = true;
	virtual InteractionBoolHolder modifiedDeathHandling(const Bullet* parent) override;

	BarrierBulletPower();
};
