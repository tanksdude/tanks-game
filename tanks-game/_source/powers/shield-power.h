#pragma once
#include "../power.h"

class ShieldPower : public Power {
public: //tank and bullet stuff
	static const double barrierStrengthPercentDrain; //percent of full time to subtract when harming the shield

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return ShieldPower::getClassName(); }
	static std::string getClassName() { return "shield"; }
	virtual ColorValueHolder getColor() const override { return ShieldPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xB6/255.0, 0xE6/255.0, 0x9D/255.0); } //approximately barrier but yellow, gray, and light
	//TODO: color blends in too much with background, hard to see

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	ShieldPower();
	static Power* factory();
};



class ShieldTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return ShieldPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override;
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesDeathHandling = true;
	virtual InteractionBoolHolder modifiedDeathHandling(const Tank* parent) override;

	ShieldTankPower();
	ShieldTankPower(double life);
};



class ShieldBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override { return ShieldPower::getClassColor(); }
	virtual std::string getColorIdentifier() const override { return ShieldPower::getClassName(); }

	virtual BulletPower* makeDuplicate() const override;
	virtual TankPower* makeTankPower() const override;

	//bool modifiesDeathHandling = true;
	virtual InteractionBoolHolder modifiedDeathHandling(const Bullet* parent) override;

	ShieldBulletPower();
	//ShieldBulletPower(double life);
};
