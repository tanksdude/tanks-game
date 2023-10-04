#pragma once
#include "../power.h"

class BlastPower : public Power {
	//called mega-blast in JS Tanks
public: //tank stuff
	static const double bulletAngleDeviation;
	static const int bulletAmount;

public: //bullet stuff
	static const double maxBulletAcceleration;
	static const double minBulletAcceleration;
	static const double degradeAmount;

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		//TODO: move to vanilla-extra?
		//JS: did have supermix (replaced by bounce)
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return BlastPower::getClassName(); }
	static std::string getClassName() { return "blast"; }
	virtual ColorValueHolder getColor() const override { return BlastPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x44/255.0, 0x66/255.0, 0x88/255.0); } //dark blue

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	BlastPower();
	static Power* factory();
};



class BlastTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BlastPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new BlastTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesAdditionalShooting = true;
	virtual void additionalShooting(Tank* parent, const CannonPoint&) override;
	//bool overridesAdditionalShooting = true;

	virtual double getTankMaxSpeedMultiplier() const override { return .5; }
	virtual double getTankAccelerationMultiplier() const override { return .5; }
	//virtual double getTankFiringRateMultiplier() const override { return .5; } //JS

	BlastTankPower();
};



class BlastBulletPower : public BulletPower {
protected:
	double accelerationAmount;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BlastPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override;
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;

	virtual bool getModifiesCollisionWithCircleHazard(const CircleHazard*) const override;
	virtual InteractionBoolHolder modifiedCollisionWithCircleHazard(Bullet*, CircleHazard*) override;

	virtual bool getModifiesCollisionWithRectHazard(const RectHazard*) const override;
	virtual InteractionBoolHolder modifiedCollisionWithRectHazard(Bullet*, RectHazard*) override;

	virtual double getBulletRadiusMultiplier() const override { return .25; } //JS: .5
	virtual double getBulletAcceleration() const override;
	virtual double getBulletDegradeAmount() const override { return BlastPower::degradeAmount; }

	BlastBulletPower();
	BlastBulletPower(double acceleration); //protected?
};
