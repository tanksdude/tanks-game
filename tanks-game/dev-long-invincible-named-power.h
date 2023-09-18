#pragma once
//#include "power.h"
#include "invincible-named-power.h"

class DevLongInvincibleNamedPower : public InvincibleNamedPower {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev" }; //no random-dev
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return DevLongInvincibleNamedPower::getClassName(); }
	static std::string getClassName() { return "longinvincible"; }
	virtual ColorValueHolder getColor() const override { return DevLongInvincibleNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return InvincibleNamedPower::getClassColor(); } //white

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	DevLongInvincibleNamedPower();
	static Power* factory();
};



class DevLongInvincibleNamedTankPower : public InvincibleNamedTankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return DevLongInvincibleNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new DevLongInvincibleNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override { return 1; }
	virtual double getTankAccelerationMultiplier() const override { return 1; }

	/*
	virtual float getOffenseImportance() const override { return 0; }
	virtual float getOffenseTier(const Tank*) const override { return 0; }
	virtual float getDefenseImportance() const override { return 0; }
	virtual float getDefenseTier(const Tank*) const override { return 2.5; }
	*/

	DevLongInvincibleNamedTankPower();
};



class DevLongInvincibleNamedBulletPower : public InvincibleNamedBulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return DevLongInvincibleNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new DevLongInvincibleNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	/*
	virtual float getOffenseImportance() const override { return 0; }
	virtual float getOffenseTier(const Bullet*) const override { return 1; }
	virtual float getDefenseImportance() const override { return 0; }
	virtual float getDefenseTier(const Bullet*) const override { return 1; }
	*/

	DevLongInvincibleNamedBulletPower();
};
