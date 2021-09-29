#pragma once
#include "power.h"

class TriplePower : public Power {
public:
	virtual std::vector<std::string> getPowerAttributes() const override {
		//might want to change how this power works (need to adjust cannonpoints)
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return TriplePower::getClassName(); }
	static std::string getClassName() { return "triple"; }
	virtual ColorValueHolder getColor() const override { return TriplePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xCC/255.0, 0xCC/255.0, 0.5f); } //beige

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	TriplePower();
	static Power* factory();
};



class TripleTankPower : public TankPower {
protected:
	static const double angleDiff;

public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return TriplePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new TripleTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesAdditionalShooting = true;
	virtual void additionalShooting(Tank* parent, CannonPoint) override;
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	TripleTankPower();
};



class TripleBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return TriplePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new TripleBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	TripleBulletPower();
};
