#pragma once
#include "power.h"

class BouncePower : public Power {
public: //bullet stuff
	static const int maxBounces;

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "supermix", "supermix-vanilla", "random" };
		//do include in old because it's from JS but not random-old because it's different
		//JS: did not have supermix (replaced by blast)
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		//I'm very conflicted on this one: while it does handle stacking (like all powers should), it's not recommended; it can mix with others, but not to great results
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return BouncePower::getClassName(); }
	static std::string getClassName() { return "bounce"; }
	virtual ColorValueHolder getColor() const override { return BouncePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.0f, 0.75f); } //pink //JS: #FF00CC

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	BouncePower();
	//TODO: virtual destructor for OldBouncePower and UltraBouncePower?
	static Power* factory();
};



class BounceTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BouncePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new BounceTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	BounceTankPower();
};



class BounceBulletPower : public BulletPower {
protected:
	int bouncesLeft;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BouncePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override;
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;

	//bool modifiesCollisionWithEdge = true;
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) override;

	virtual double getBulletSpeedMultiplier() const override { return .5; }

	BounceBulletPower();
	BounceBulletPower(int bounces);
};
