#pragma once
#include "../power.h"

class TricksterCirclePower : public Power {
public: //bullet stuff
	static const double timePeriod;
	static const double movementRadius;

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

	virtual std::string getName() const override { return TricksterCirclePower::getClassName(); }
	static std::string getClassName() { return "trickster_circle"; }
	virtual ColorValueHolder getColor() const override { return TricksterCirclePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x00/255.0, 0x16/255.0, 0x59/255.0); } //really dark blue (too close to annoying)

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	TricksterCirclePower();
	static Power* factory();
};



class TricksterCircleTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return TricksterCirclePower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new TricksterCircleTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	TricksterCircleTankPower();
};



class TricksterCircleBulletPower : public BulletPower {
protected:
	double tickCount;

public:
	virtual ColorValueHolder getColor() const override { return TricksterCirclePower::getClassColor(); }
	virtual std::string getColorIdentifier() const override { return TricksterCirclePower::getClassName(); }

	virtual BulletPower* makeDuplicate() const override { return new TricksterCircleBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;

	virtual float getBulletSpeedMultiplier() const override { return .25; }

	TricksterCircleBulletPower();
};
