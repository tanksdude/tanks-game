#pragma once
#include "power.h"

//TODO: this should be renamed (but probably won't...)
class MinesPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		//conflicted on this one, like bounce, though leaning much more towards "doesn't stack" than "does stack"
		//also somewhat conflicted on mixing because it just doesn't mix well
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return MinesPower::getClassName(); }
	static std::string getClassName() { return "mines"; }
	virtual ColorValueHolder getColor() const override { return MinesPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0, 0, 0); } //black, so it's hard to see where the tank is pointed
	virtual double getColorImportance() const override { return MinesPower::getClassColorImportance(); }
	static double getClassColorImportance() { return .5; } //godmode can overpower this

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	MinesPower();
	static Power* factory();
};



class MinesTankPower : public TankPower {
protected:
	static const double bulletDistance; //percentage from center (beginning of cannon) to end of cannon

public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual void tick(Tank* t) override; //for updating modifiesAdditionalShooting based on whether there are any other powers that modify additionalShooting
	//might need a secondary tick for this; one tick to do stuff, another tick to update interaction bools

	virtual ColorValueHolder getColor() const override {
		return MinesPower::getClassColor();
	}
	virtual double getColorImportance() const override {
		return MinesPower::getClassColorImportance();
	}

	virtual TankPower* makeDuplicate() const override { return new MinesTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint) override;
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	virtual double getTankFiringRateMultiplier() const override { return .25; } //.5?
	
	MinesTankPower();
};



class MinesBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return MinesPower::getClassColor();
	}
	virtual double getColorImportance() const override {
		return MinesPower::getClassColorImportance();
	}

	virtual BulletPower* makeDuplicate() const override { return new MinesBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	virtual double getBulletSpeedMultiplier() const override { return 0; }
	//bool bulletSpeedStacks = false; //true?
	virtual double getBulletAcceleration() const override { return 0; }
	virtual double getBulletAccelerationImportance() const override { return .5; }

	MinesBulletPower();
};
