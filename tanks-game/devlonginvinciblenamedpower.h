#pragma once
#include "power.h"

class DevLongInvincibleNamedPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev" }; //no random-dev
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "dev", 0.0f }); //this should not appear, like, ever
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return DevLongInvincibleNamedPower::getClassName(); }
	static std::string getClassName() { return "longinvincible"; }
	virtual ColorValueHolder getColor() const override { return DevLongInvincibleNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 1.0f, 1.0f); } //white

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	DevLongInvincibleNamedPower();
	static Power* factory();
};



class DevLongInvincibleNamedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return DevLongInvincibleNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new DevLongInvincibleNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override { return .5; }
	virtual double getTankAccelerationMultiplier() const override { return .5; }

	virtual double getOffenseImportance() const override { return 0; }
	virtual double getOffenseTier(Tank*) const override { return 0; }
	virtual double getDefenseImportance() const override { return 0; }
	virtual double getDefenseTier(Tank*) const override { return 2.5; }

	DevLongInvincibleNamedTankPower();
};



class DevLongInvincibleNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return DevLongInvincibleNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new DevLongInvincibleNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	virtual double getOffenseImportance() const override { return 0; }
	virtual double getOffenseTier(Bullet*) const override { return 1; }
	virtual double getDefenseImportance() const override { return 0; }
	virtual double getDefenseTier(Bullet*) const override { return 1; }

	DevLongInvincibleNamedBulletPower();
};
