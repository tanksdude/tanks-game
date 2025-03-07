#pragma once
#include "invincible-named-power.h"

class DevLongInvincibleNamedPower : public InvincibleNamedPower {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev" }; //no random-dev
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return DevLongInvincibleNamedPower::getClassName(); }
	static std::string getClassName() { return "longinvincible"; }
	virtual std::string getIdentifier() const override { return DevLongInvincibleNamedPower::getClassIdentifier(); }
	static std::string getClassIdentifier() { return DevLongInvincibleNamedPower::getClassName(); }
	virtual ColorValueHolder getColor() const override { return DevLongInvincibleNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return InvincibleNamedPower::getClassColor(); } //white

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	DevLongInvincibleNamedPower();
	static Power* factory();
};



class DevLongInvincibleNamedTankPower : public InvincibleNamedTankPower {
public:
	virtual std::string getIdentifier() const override { return DevLongInvincibleNamedPower::getClassIdentifier(); }
	virtual ColorValueHolder getColor() const override { return DevLongInvincibleNamedPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new DevLongInvincibleNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override { return 1; }
	virtual double getTankAccelerationMultiplier() const override { return 1; }

	DevLongInvincibleNamedTankPower();
};



class DevLongInvincibleNamedBulletPower : public InvincibleNamedBulletPower {
public:
	virtual std::string getIdentifier() const override { return DevLongInvincibleNamedPower::getClassIdentifier(); }
	virtual ColorValueHolder getColor() const override { return DevLongInvincibleNamedPower::getClassColor(); }

	virtual BulletPower* makeDuplicate() const override { return new DevLongInvincibleNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	DevLongInvincibleNamedBulletPower();
};
