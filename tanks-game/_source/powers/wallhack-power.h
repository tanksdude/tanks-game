#pragma once
#include "../power.h"

class WallhackPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "ultimate", "ultimate-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return WallhackPower::getClassName(); }
	static std::string getClassName() { return "wallhack"; } //"wallHack" in JS Tanks
	virtual ColorValueHolder getColor() const override { return WallhackPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 1.0f, 0.0f); } //yellow

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	WallhackPower();
	static Power* factory();
};



class WallhackTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override {
		return WallhackPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new WallhackTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Tank*, Wall*) override;
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;

	WallhackTankPower();
};



class WallhackBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override {
		return WallhackPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new WallhackBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;

	WallhackBulletPower();
};
