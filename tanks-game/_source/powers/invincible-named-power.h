#pragma once
#include "../power.h"

class InvincibleNamedPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
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

	InvincibleNamedPower();
	//TODO: virtual destructor for DevLongInvincibleNamedPower?
	static Power* factory();
};



class InvincibleNamedTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return InvincibleNamedPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new InvincibleNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual float getTankMaxSpeedMultiplier() const override { return .5; }
	virtual float getTankAccelerationMultiplier() const override { return .5; } //JS: 1

	virtual float getOffenseTier(const Tank*) const override { return 0; }
	virtual float getDefenseTier(const Tank*) const override { return 2.5; } // this is not DESTRUCTION_TIER+.5 in case someone wants to have a higher destruction tier

	InvincibleNamedTankPower();
};



class InvincibleNamedBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override { return InvincibleNamedPower::getClassColor(); }
	virtual std::string getColorIdentifier() const override { return InvincibleNamedPower::getClassName(); }

	virtual BulletPower* makeDuplicate() const override { return new InvincibleNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	virtual float getOffenseTier(const Bullet*) const override { return 1; }
	virtual float getDefenseTier(const Bullet*) const override { return 1; }

	InvincibleNamedBulletPower();
};
