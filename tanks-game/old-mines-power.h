#pragma once
//#include "power.h"
#include "mines-power.h"

class OldMinesPower : public MinesPower {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old" };
		//no random-old
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	//virtual std::vector<std::string> getPowerAttributes() const override; //follows MinesPower

	virtual std::string getName() const override { return OldMinesPower::getClassName(); }
	static std::string getClassName() { return "old_mines"; }
	virtual ColorValueHolder getColor() const override { return OldMinesPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return MinesPower::getClassColor(); } //black, so it's hard to see where the tank is pointed
	virtual double getColorImportance() const override { return OldMinesPower::getClassColorImportance(); }
	static double getClassColorImportance() { return MinesPower::getClassColorImportance(); } //not supposed to have color importance but whatever

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	OldMinesPower();
	static Power* factory();
};



class OldMinesTankPower : public MinesTankPower {
protected:
	//static const double bulletDistance; //percentage from center (beginning of cannon) to end of cannon

public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	/*
	virtual void tick(Tank* t) override; //for updating modifiesAdditionalShooting based on whether there are any other powers that modify additionalShooting
	//might need a secondary tick for this; one tick to do stuff, another tick to update interaction bools
	*/

	virtual ColorValueHolder getColor() const override {
		return OldMinesPower::getClassColor();
	}
	virtual double getColorImportance() const override {
		return OldMinesPower::getClassColorImportance();
	}

	virtual TankPower* makeDuplicate() const override { return new OldMinesTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	/*
	//bool modifiesAdditionalShooting = true;
	virtual void additionalShooting(Tank* parent, CannonPoint) override;
	//bool overridesAdditionalShooting = true;
	*/

	//virtual double getTankFiringRateMultiplier() const override { return .25; } //.5?
	//bool tankFiringRateStacks = true;

	OldMinesTankPower();
};



class OldMinesBulletPower : public MinesBulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return OldMinesPower::getClassColor();
	}
	virtual double getColorImportance() const override {
		return OldMinesPower::getClassColorImportance();
	}

	virtual BulletPower* makeDuplicate() const override { return new OldMinesBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	/*
	virtual double getBulletSpeedMultiplier() const override { return 0; }
	//bool bulletSpeedStacks = false; //true?
	virtual double getBulletAcceleration() const override { return 0; }
	virtual double getBulletAccelerationImportance() const override { return .5; }
	*/

	OldMinesBulletPower();
};
