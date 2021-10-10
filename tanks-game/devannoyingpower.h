#pragma once
#include "power.h"

//TODO: should there be a non-dev annoying power which has basic stuff and this one has *everything* annoying (like touch a wall and die)?
//... should it be renamed to "silly"? should there be a separate power?

class DevAnnoyingPower : public Power {
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

	virtual std::string getName() const override { return DevAnnoyingPower::getClassName(); }
	static std::string getClassName() { return "annoying"; }
	virtual ColorValueHolder getColor() const override { return DevAnnoyingPower::getClassColor(); } //must be indigo because it's annoying trying to distinguish it from purple and/or blue
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x4B/255.0, 0x00/255.0f, 0x82/255.0f); } //indigo (#4E1885 is pretty close)

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	DevAnnoyingPower();
	static Power* factory();
};



class DevAnnoyingTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return DevAnnoyingPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new DevAnnoyingTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesEdgeCollision = true;
	virtual InteractionBoolHolder modifiedEdgeCollision(Tank*) override;
	//precondition: was out-of-bounds, is not necessarily out-of-bounds
	//bool overridesEdgeCollision = true;
	//bool modifiedEdgeCollisionCanWorkWithOthers = true;
	//bool modifiedEdgeCollisionCanOnlyWorkIndividually = false;

	DevAnnoyingTankPower();
};



class DevAnnoyingBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return DevAnnoyingPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new DevAnnoyingBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithTank = true;
	virtual InteractionBoolHolder modifiedCollisionWithTank(Bullet*, Tank*) override;
	//bool overridesCollisionWithTank = true;
	//bool modifiedCollisionWithTankCanWorkWithOthers = true;
	//bool modifiedCollisionWithTankCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() const override { return .5; }

	virtual double getOffenseImportance() const override;
	virtual double getOffenseTier(Bullet*) const override;
	virtual double getDefenseImportance() const override;
	virtual double getDefenseTier(Bullet*) const override;

	DevAnnoyingBulletPower();
};
