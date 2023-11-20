#pragma once
#include "banana-power.h"

class BananaSplitPower : public BananaPower {
	//note: not named after the dessert with the same name; "cluster split" is more accurate, but it's too late to change banana's name
public: //bullet stuff
	static const int bananaSplitCount;
	static const double bananaSplitAngleDeviation;

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return BananaSplitPower::getClassName(); }
	static std::string getClassName() { return "banana_split"; }
	virtual ColorValueHolder getColor() const override { return BananaSplitPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x99/255.0, 0xBB/255.0, 0x33/255.0); } //dull gold with more green (because that's shotgun's color)

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	BananaSplitPower();
	static Power* factory();
};



class BananaSplitTankPower : public BananaTankPower {
public:
	virtual ColorValueHolder getColor() const override {
		return BananaSplitPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new BananaSplitTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	BananaSplitTankPower();
};



class BananaSplitBulletPower : public BananaBulletPower {
protected:
	virtual void bananaExplode(const Bullet*) override;

public:
	virtual ColorValueHolder getColor() const override {
		return BananaSplitPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new BananaSplitBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	BananaSplitBulletPower();
};
