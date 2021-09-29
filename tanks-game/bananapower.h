#pragma once
#include "power.h"

class BananaPower : public Power {
public:
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
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BananaPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new BananaTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	BananaTankPower();
};



class BananaBulletPower : public BulletPower {
protected:
	static const int bananaCount;
	static const double maxNewBulletVelocity;
	static const double minNewBulletVelocity;
	//bool wasStationary; //must have velocity of zero for one tick before banana-ing

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BananaPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new BananaBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	virtual double getBulletAcceleration() const override { return -1.0/8; }

	BananaBulletPower();
};
