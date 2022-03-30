#pragma once
#include "power.h"

class DevBackwardsMovementPower : public Power {
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

	virtual std::string getName() const override { return DevBackwardsMovementPower::getClassName(); }
	static std::string getClassName() { return "backwards_movement"; }
	virtual ColorValueHolder getColor() const override { return DevBackwardsMovementPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x52/255.0, 0xAA/255.0, 0xDD/255.0); } //light blue-ish

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	DevBackwardsMovementPower();
	static Power* factory();
};



class DevBackwardsMovementTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return DevBackwardsMovementPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new DevBackwardsMovementTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Tank*) override;

	//virtual double getTankAccelerationMultiplier() const override { return .5; }

	DevBackwardsMovementTankPower();
};



class DevBackwardsMovementBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return DevBackwardsMovementPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new DevBackwardsMovementBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	DevBackwardsMovementBulletPower();
};
