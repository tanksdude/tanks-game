#pragma once
#include "../power.h"

class DevMovementRestarterPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return DevMovementRestarterPower::getClassName(); }
	static std::string getClassName() { return "movement_restarter"; }
	virtual ColorValueHolder getColor() const override { return DevMovementRestarterPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x83/255.0, 0x89/255.0, 0x96/255.0); } //"roman silver" apparently

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	DevMovementRestarterPower();
	static Power* factory();
};



class DevMovementRestarterTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return DevMovementRestarterPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new DevMovementRestarterTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	DevMovementRestarterTankPower();
};



class DevMovementRestarterBulletPower : public BulletPower {
protected:
	bool hasRestartedMovement; //only restarts movement once

public:
	virtual ColorValueHolder getColor() const override { return DevMovementRestarterPower::getClassColor(); }

	virtual BulletPower* makeDuplicate() const override { return new DevMovementRestarterBulletPower(); } //don't pass on hasRestartedMovement
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;

	DevMovementRestarterBulletPower();
};
