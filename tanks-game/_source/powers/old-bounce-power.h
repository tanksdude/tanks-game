#pragma once
//#include "../power.h"
#include "bounce-power.h"

class OldBouncePower : public BouncePower {
public: //bullet stuff
	//static const int maxBounces;

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	//virtual std::vector<std::string> getPowerAttributes() const override;

	virtual std::string getName() const override { return OldBouncePower::getClassName(); }
	static std::string getClassName() { return "old_bounce"; }
	virtual ColorValueHolder getColor() const override { return OldBouncePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return BouncePower::getClassColor(); } //pink

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	OldBouncePower();
	static Power* factory();
};



class OldBounceTankPower : public BounceTankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return OldBouncePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new OldBounceTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankAccelerationMultiplier() const override { return .5; }
	virtual double getTankRadiusMultiplier() const override { return .5; } //JS: stacks (as did everything)
	virtual double getTankFiringRateMultiplier() const override { return .5; }

	OldBounceTankPower();
};



class OldBounceBulletPower : public BounceBulletPower {
protected:
	//int bouncesLeft;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return OldBouncePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new OldBounceBulletPower(); } //I don't think bounces were passed on in JS
	virtual TankPower* makeTankPower() const override;

	/*
	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;

	//bool modifiesCollisionWithEdge = true;
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) override;
	*/

	virtual double getBulletSpeedMultiplier() const override { return .25; }

	OldBounceBulletPower();
};
