#pragma once
//#include "power.h"
#include "big-named-power.h"

class OldBigNamedPower : public BigNamedPower {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	//virtual std::vector<std::string> getPowerAttributes() const override;

	virtual std::string getName() const override { return OldBigNamedPower::getClassName(); }
	static std::string getClassName() { return "old_big"; }
	virtual ColorValueHolder getColor() const override { return OldBigNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return BigNamedPower::getClassColor(); } //red

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	OldBigNamedPower();
	static Power* factory();
};



class OldBigNamedTankPower : public BigNamedTankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return OldBigNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new OldBigNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	/*
	virtual double getTankMaxSpeedMultiplier() const override { return .5; }
	virtual double getTankAccelerationMultiplier() const override { return .5; }
	virtual double getTankFiringRateMultiplier() const override { return 4; } //stacked in JS, not sure if it should stack here
	*/

	OldBigNamedTankPower();
};



#include "constants.h"

class OldBigNamedBulletPower : public BigNamedBulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return OldBigNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new OldBigNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	/*
	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;
	*/

	virtual double getBulletSpeedMultiplier() const override { return .5; }
	//bool bulletSpeedStacks = true;
	//virtual double getBulletRadiusMultiplier() const override { return 4; }
	//bool bulletRadiusStacks = true;

	/*
	virtual float getOffenseImportance() const override { return 0; }
	virtual float getOffenseTier(const Bullet*) const override { return DESTRUCTION_TIER; }
	virtual float getDefenseImportance() const override { return 0; }
	virtual float getDefenseTier(const Bullet*) const override { return DESTRUCTION_TIER; }
	*/

	OldBigNamedBulletPower();
};
