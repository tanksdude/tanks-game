#pragma once
#include "../power.h"

class TricksterSnakePower : public Power {
public: //bullet stuff
	static const double timePeriod;
	static const double movementAmplitude;

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

	virtual std::string getName() const override { return TricksterSnakePower::getClassName(); }
	static std::string getClassName() { return "trickster_snake"; }
	virtual ColorValueHolder getColor() const override { return TricksterSnakePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xD9/255.0, 0xB3/255.0, 0xFF/255.0); } //really faded purple-ish

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	TricksterSnakePower();
	static Power* factory();
};



class TricksterSnakeTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override {
		return TricksterSnakePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new TricksterSnakeTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	TricksterSnakeTankPower();
};



class TricksterSnakeBulletPower : public BulletPower {
protected:
	double tickCount;

public:
	virtual ColorValueHolder getColor() const override {
		return TricksterSnakePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new TricksterSnakeBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;

	//virtual double getBulletSpeedMultiplier() const override { return .5; } //maybe?

	TricksterSnakeBulletPower();
};
