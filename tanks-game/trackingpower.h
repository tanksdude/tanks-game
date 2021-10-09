#pragma once
#include "power.h"

class TrackingPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla-extra", .25f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() const override {
		//doesn't mix well
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return TrackingPower::getClassName(); }
	static std::string getClassName() { return "tracking"; }
	virtual ColorValueHolder getColor() const override { return TrackingPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xDD/255.0, 0.25f, 0.25f); } //faded red-orange ("maroon")

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	TrackingPower();
	static Power* factory();
};



class TrackingTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return TrackingPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new TrackingTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override { return .5; }
	virtual double getTankAccelerationMultiplier() const override { return .5; }
	virtual double getTankFiringRateMultiplier() const override { return 2; }

	TrackingTankPower();
};



class TrackingBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return TrackingPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new TrackingBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = false;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true; //false means also use the default
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() const override { return .25; }

	TrackingBulletPower();
};
