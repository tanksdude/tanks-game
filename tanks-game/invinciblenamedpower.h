#pragma once
#include "power.h"

class InvincibleNamedPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla", .5f });
		weights.insert({ "random-vanilla", .5f });
		weights.insert({ "random", .5f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return InvincibleNamedPower::getClassName(); }
	static std::string getClassName() { return "invincible"; }
	virtual ColorValueHolder getColor() const override { return InvincibleNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 1.0f, 1.0f); } //white

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	InvincibleNamedPower();
	static Power* factory();
};



class InvincibleNamedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return InvincibleNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new InvincibleNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override { return .5; }
	virtual double getTankAccelerationMultiplier() const override { return .5; }
	
	virtual double getOffenseImportance() const override { return 0; }
	virtual double getOffenseTier(Tank*) const override { return 0; }
	virtual double getDefenseImportance() const override { return 0; }
	virtual double getDefenseTier(Tank*) const override { return 2.5; } // this is not DESTRUCTION_TIER+.5 in case someone wants to have a higher destruction tier

	InvincibleNamedTankPower();
};



class InvincibleNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return InvincibleNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new InvincibleNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	virtual double getOffenseImportance() const override { return 0; }
	virtual double getOffenseTier(Bullet*) const override { return 1; }
	virtual double getDefenseImportance() const override { return 0; }
	virtual double getDefenseTier(Bullet*) const override { return 1; }

	InvincibleNamedBulletPower();
};
