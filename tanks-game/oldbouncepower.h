#pragma once
#include "power.h"

class OldBouncePower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		//match with BouncePower
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return OldBouncePower::getClassName(); }
	static std::string getClassName() { return "old_bounce"; }
	virtual ColorValueHolder getColor() const override { return OldBouncePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.0f, 0xCC/255.0); } //pink

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	OldBouncePower();
	static Power* factory();
};



class OldBounceTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return OldBouncePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new OldBounceTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankAccelerationMultiplier() const override { return .5; }
	virtual double getTankRadiusMultiplier() const override { return .5; } //should this stack?
	virtual double getTankFiringRateMultiplier() const override { return .5; }

	OldBounceTankPower();
};



class OldBounceBulletPower : public BulletPower {
protected:
	static const short maxBounces;
	short bouncesLeft;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return OldBouncePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new OldBounceBulletPower(); } //I don't think bounces were passed on in JS
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithEdge = true;
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) override;
	//bool overridesEdgeCollision = true;
	//bool modifiedEdgeCollisionCanWorkWithOthers = false;
	//bool modifiedEdgeCollisionCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() const override { return .25; }

	OldBounceBulletPower();
};
