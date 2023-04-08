#pragma once
#include "power.h"

class BigNamedPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random" };
		//do include in old because it's from JS but not random-old because it's different
		//include ultimate?
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	//virtual std::vector<std::string> getPowerAttributes() const override;

	virtual std::string getName() const override { return BigNamedPower::getClassName(); }
	static std::string getClassName() { return "big"; }
	virtual ColorValueHolder getColor() const override { return BigNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.0f, 0.0f); } //red

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	BigNamedPower();
	//TODO: virtual destructor for OldBigNamedPower?
	static Power* factory();
};



class BigNamedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BigNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new BigNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override { return .5; }
	virtual double getTankAccelerationMultiplier() const override { return .5; }
	virtual double getTankFiringRateMultiplier() const override { return 4; } //maybe *2?

	BigNamedTankPower();
};



#include "constants.h"

class BigNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BigNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new BigNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;

	virtual double getBulletSpeedMultiplier() const override { return .25; }
	virtual double getBulletRadiusMultiplier() const override { return 4; }
	//bool bulletRadiusStacks = true;

	virtual double getOffenseImportance() const override { return 0; }
	virtual double getOffenseTier(const Bullet*) const override { return DESTRUCTION_TIER; }
	virtual double getDefenseImportance() const override { return 0; }
	virtual double getDefenseTier(const Bullet*) const override { return DESTRUCTION_TIER; }

	BigNamedBulletPower();
};
