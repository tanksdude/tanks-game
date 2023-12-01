#pragma once
#include "mines-power.h"

class OldMinesPower : public MinesPower {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old" }; //no random-old
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return OldMinesPower::getClassName(); }
	static std::string getClassName() { return "old_mines"; }
	virtual ColorValueHolder getColor() const override { return OldMinesPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return MinesPower::getClassColor(); } //black, so it's hard to see where the tank is pointed
	virtual float getColorImportance() const override { return OldMinesPower::getClassColorImportance(); }
	static float getClassColorImportance() { return MinesPower::getClassColorImportance(); } //JS: color importance didn't exist (except for rainbow)

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	OldMinesPower();
	static Power* factory();
};



class OldMinesTankPower : public MinesTankPower {
public:
	virtual ColorValueHolder getColor() const override {
		return OldMinesPower::getClassColor();
	}
	virtual float getColorImportance() const override {
		return OldMinesPower::getClassColorImportance();
	}

	virtual TankPower* makeDuplicate() const override { return new OldMinesTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool tankFiringRateStacks = true; //this entire power exists only to change this, just so mine heaven level works

	OldMinesTankPower();
};



class OldMinesBulletPower : public MinesBulletPower {
public:
	virtual ColorValueHolder getColor() const override {
		return OldMinesPower::getClassColor();
	}
	virtual float getColorImportance() const override {
		return OldMinesPower::getClassColorImportance();
	}

	virtual BulletPower* makeDuplicate() const override { return new OldMinesBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	OldMinesBulletPower();
};
