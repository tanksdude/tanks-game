#pragma once
#include "../power.h"

class ShotgunPower : public Power {
public: //tank stuff
	static const int bulletSpreadCount; //number of extra bullets, *2 (shotgun shoots four extra so this number is 2)

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "old", "random-old", "random" };
		//JS: did have supermix
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return ShotgunPower::getClassName(); }
	static std::string getClassName() { return "shotgun"; }
	virtual ColorValueHolder getColor() const override { return ShotgunPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.0f, 1.0f, 0.25f); } //green //JS: #00FF44

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	ShotgunPower();
	static Power* factory();
};



class ShotgunTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return ShotgunPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new ShotgunTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesAdditionalShooting = true;
	virtual void additionalShooting(Tank* parent, const CannonPoint&) override;

	//virtual double getTankFiringRateMultiplier() const override { return .5; }

	ShotgunTankPower();
};



class ShotgunBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return ShotgunPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new ShotgunBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	virtual double getBulletSpeedMultiplier() const override { return .5; }

	ShotgunBulletPower();
};
