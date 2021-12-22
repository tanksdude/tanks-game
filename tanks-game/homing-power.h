#pragma once
#include "power.h"

class HomingPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	//virtual std::vector<std::string> getPowerAttributes() const override; //mix only?

	virtual std::string getName() const override { return HomingPower::getClassName(); }
	static std::string getClassName() { return "homing"; }
	virtual ColorValueHolder getColor() const override { return HomingPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xBB/255.0, 0x66/255.0, 0xAA/255.0); } //really faded purple

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	HomingPower();
	static Power* factory();
};



class HomingTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return HomingPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new HomingTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	HomingTankPower();
};



class HomingBulletPower : public BulletPower {
protected: //public?
	static const double homingStrength;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return HomingPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new HomingBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;

	//bool modifiesCollisionWithEdge = true;
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) override;

	virtual double getBulletSpeedMultiplier() const override { return .5; }

	HomingBulletPower();
};
