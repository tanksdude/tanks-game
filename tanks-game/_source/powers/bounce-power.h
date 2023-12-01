#pragma once
#include "../power.h"

//TOOD: wall/edge bouncing should probably be a built-in feature, since multiple powers bounce, but they can "double bounce" off stuff which is very wrong
//wall bounce notes: check if the bulletpower does bounce, do the bounce, apply an update to the bulletpower (to decrease bounce count), check if the other bulletpowers want to do something
//if the other bulletpowers do want to do something, then only the first one can do anything to the wall (so ultrabounce doesn't push twice)
//should something like wall sparks create twice as many sparks, or should it only be allowed once? could this whole thing be solved by not letting the same power type go twice?

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
	virtual ColorValueHolder getColor() const override {
		return BouncePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new BounceTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//virtual double getTankAccelerationMultiplier() const override { return .5; } //JS
	//virtual double getTankRadiusMultiplier() const override { return .5; } //JS
	//virtual double getTankFiringRateMultiplier() const override { return .5; } //JS

	BounceTankPower();
};



class BounceBulletPower : public BulletPower {
protected:
	int bouncesLeft;

public:
	virtual ColorValueHolder getColor() const override {
		return BouncePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override;
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;

	//bool modifiesCollisionWithEdge = true;
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) override;

	virtual double getBulletSpeedMultiplier() const override { return .5; } //JS: .25

	BounceBulletPower();
	BounceBulletPower(int bounces);
};
