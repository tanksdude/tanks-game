#pragma once
#include "dev-long-invincible-named-power.h"

#include "../constants.h"

class DevColorlessLongInvincibleNamedPower : public DevLongInvincibleNamedPower {
public:
	virtual std::string getName() const override { return DevColorlessLongInvincibleNamedPower::getClassName(); }
	static std::string getClassName() { return "colorless_longinvincible"; }
	virtual ColorValueHolder getColor() const override { return DevColorlessLongInvincibleNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return DevLongInvincibleNamedPower::getClassColor(); } //white
	virtual float getColorImportance() const override { return DevColorlessLongInvincibleNamedPower::getClassColorImportance(); }
	static float getClassColorImportance() { return LOW_IMPORTANCE; } //cheat to never show up

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	DevColorlessLongInvincibleNamedPower();
	static Power* factory();
};



class DevColorlessLongInvincibleNamedTankPower : public DevLongInvincibleNamedTankPower {
public:
	virtual ColorValueHolder getColor() const override { return DevColorlessLongInvincibleNamedPower::getClassColor(); }
	virtual float getColorImportance() const override { return DevColorlessLongInvincibleNamedPower::getClassColorImportance(); }

	virtual TankPower* makeDuplicate() const override { return new DevColorlessLongInvincibleNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	DevColorlessLongInvincibleNamedTankPower();
};



class DevColorlessLongInvincibleNamedBulletPower : public DevLongInvincibleNamedBulletPower {
public:
	virtual ColorValueHolder getColor() const override { return DevColorlessLongInvincibleNamedPower::getClassColor(); }
	virtual float getColorImportance() const override { return DevColorlessLongInvincibleNamedPower::getClassColorImportance(); }
	virtual std::string getColorIdentifier() const override { return DevColorlessLongInvincibleNamedPower::getClassName(); }

	virtual BulletPower* makeDuplicate() const override { return new DevColorlessLongInvincibleNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	DevColorlessLongInvincibleNamedBulletPower();
};
