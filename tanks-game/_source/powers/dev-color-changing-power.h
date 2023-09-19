#pragma once
#include "../power.h"

class DevColorChangingPower : public Power {
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

	virtual std::string getName() const override { return DevColorChangingPower::getClassName(); }
	static std::string getClassName() { return "color_changing"; }
	virtual ColorValueHolder getColor() const override; //powersquares don't have a tick() ability...
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.5f, 0.5f, 0.5f); } //default/fallback
	virtual float getColorImportance() const override { return DevColorChangingPower::getClassColorImportance(); }
	static float getClassColorImportance() { return .5; }

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	DevColorChangingPower();
	static Power* factory();
};



class DevColorChangingTankPower : public TankPower {
protected:
	double colorDist;

public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual void tick(Tank* t) override; //updates color

	virtual ColorValueHolder getColor() const override;
	virtual float getColorImportance() const override {
		return DevColorChangingPower::getClassColorImportance();
	}

	virtual TankPower* makeDuplicate() const override { return new DevColorChangingTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	DevColorChangingTankPower();
};



class DevColorChangingBulletPower : public BulletPower {
protected:
	double colorDist;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual void tick(Bullet* b) override; //updates color

	virtual ColorValueHolder getColor() const override;
	virtual float getColorImportance() const override {
		return DevColorChangingPower::getClassColorImportance();
	}

	virtual BulletPower* makeDuplicate() const override { return new DevColorChangingBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	DevColorChangingBulletPower();
};
