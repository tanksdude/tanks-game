#pragma once
#include "power.h"

class TripleNamedPower : public Power {
public: //tank stuff
	static const double bulletAngleDiff;

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return TripleNamedPower::getClassName(); }
	static std::string getClassName() { return "triple"; }
	virtual ColorValueHolder getColor() const override { return TripleNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.75f, 0.75f, 0.5f); } //beige //JS: #CCCC88

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	TripleNamedPower();
	static Power* factory();
};



class TripleNamedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return TripleNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new TripleNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesAdditionalShooting = true;
	virtual void additionalShooting(Tank* parent, const CannonPoint&) override;

	TripleNamedTankPower();
};



class TripleNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return TripleNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new TripleNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	TripleNamedBulletPower();
};
