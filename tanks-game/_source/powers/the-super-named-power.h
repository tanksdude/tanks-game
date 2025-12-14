#pragma once
#include "../power.h"

class TheSuperNamedPower : public Power {
public:
	static inline float getTimeValue();

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

	virtual std::string getName() const override { return TheSuperNamedPower::getClassName(); }
	static std::string getClassName() { return "the_super"; }
	virtual ColorValueHolder getColor() const override { return TheSuperNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor(); //black and white cycle
	virtual float getColorImportance() const override { return TheSuperNamedPower::getClassColorImportance(); }
	static float getClassColorImportance() { return 1; } //same as Godmode

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	TheSuperNamedPower();
	static Power* factory();
};



#include "../constants.h"

class TheSuperNamedTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return TheSuperNamedPower::getClassColor(); }
	virtual float getColorImportance() const override { return TheSuperNamedPower::getClassColorImportance(); }

	virtual TankPower* makeDuplicate() const override { return new TheSuperNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual float getOffenseTier(const Tank*) const override { return 0; }
	virtual float getDefenseTier(const Tank*) const override { return HIGH_TIER; }

	TheSuperNamedTankPower();
};



#include "../constants.h"

class TheSuperNamedBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override { return TheSuperNamedPower::getClassColor(); }
	virtual float getColorImportance() const override { return TheSuperNamedPower::getClassColorImportance(); }
	virtual std::string getColorIdentifier() const override { return TheSuperNamedPower::getClassName(); }

	virtual BulletPower* makeDuplicate() const override { return new TheSuperNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;

	virtual float getOffenseTier(const Bullet*) const override { return HIGH_TIER - .1f; }
	virtual float getDefenseTier(const Bullet*) const override { return HIGH_TIER - .1f; }

	TheSuperNamedBulletPower();
};
