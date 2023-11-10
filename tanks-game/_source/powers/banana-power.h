#pragma once
#include "../power.h"

class BananaPower : public Power {
public: //bullet stuff
	static const int bananaCount; //TODO: ability to change this in constructor
	static const double maxNewBulletVelocityMultiplier;
	static const double minNewBulletVelocityMultiplier;

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random" };
		//include ultimate?
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	//virtual std::vector<std::string> getPowerAttributes() const override;

	virtual std::string getName() const override { return BananaPower::getClassName(); }
	static std::string getClassName() { return "banana"; }
	virtual ColorValueHolder getColor() const override { return BananaPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x99/255.0, 0x99/255.0, 0x33/255.0); } //dull gold

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	BananaPower();
	static Power* factory();
};



class BananaTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override {
		return BananaPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new BananaTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	BananaTankPower();
};



class BananaBulletPower : public BulletPower {
protected:
	bool wasStationary; //must have velocity of zero for one tick before banana-ing

	virtual void bananaExplode(Bullet*);

public:
	virtual ColorValueHolder getColor() const override {
		return BananaPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new BananaBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;

	virtual double getBulletAcceleration() const override { return -1.0/8; }

	BananaBulletPower();
};
