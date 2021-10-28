#pragma once
#include "power.h"

class InversionPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return InversionPower::getClassName(); }
	static std::string getClassName() { return "inversion"; }
	virtual ColorValueHolder getColor() const override { return InversionPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.75f, 0.75f, 0.875f); } //silver-teal, I think

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	InversionPower();
	static Power* factory();
};



class InversionTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return InversionPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new InversionTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//maybe use this?:
	//bool modifiesMovement = true;
	//virtual InteractionBoolHolder modifiedMovement(Tank*) override;
	//bool overridesMovement = true;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	virtual double getTankTurningIncrementMultiplier() const override { return -1; }
	//bool tankTurningIncrementStacks = true;

	InversionTankPower();
};



class InversionBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return InversionPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new InversionBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	InversionBulletPower();
};
