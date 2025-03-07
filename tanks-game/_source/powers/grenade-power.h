#pragma once
#include "../power.h"

class GrenadePower : public Power {
public: //bullet stuff
	static const double degradeAmount;
	static const double growAmount;

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return GrenadePower::getClassName(); }
	static std::string getClassName() { return "grenade"; }
	virtual std::string getIdentifier() const override { return GrenadePower::getClassIdentifier(); }
	static std::string getClassIdentifier() { return GrenadePower::getClassName(); }
	virtual ColorValueHolder getColor() const override { return GrenadePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.75f, 1.0f, 0.0f); } //lime green //JS: #CCFF00

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	GrenadePower();
	static Power* factory();
};



class GrenadeTankPower : public TankPower {
public:
	virtual std::string getIdentifier() const override { return GrenadePower::getClassIdentifier(); }
	virtual ColorValueHolder getColor() const override { return GrenadePower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new GrenadeTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	GrenadeTankPower();
};



class GrenadeBulletPower : public BulletPower {
public:
	virtual std::string getIdentifier() const override { return GrenadePower::getClassIdentifier(); }
	virtual ColorValueHolder getColor() const override { return GrenadePower::getClassColor(); }

	virtual BulletPower* makeDuplicate() const override { return new GrenadeBulletPower(); } //should current size be passed on?
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;

	virtual double getBulletAcceleration() const override { return -1.0/16; }
	virtual double getBulletDegradeAmount() const override { return GrenadePower::degradeAmount; }
	virtual double getBulletRadiusGrowNumber_Stationary() const override { return GrenadePower::growAmount; }
	//bool bulletRadiusGrowMultiplies_Stationary = true;

	GrenadeBulletPower();
};
