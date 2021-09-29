#pragma once
#include "power.h"

class FireNamedPower : public Power {
public:
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return FireNamedPower::getClassName(); }
	static std::string getClassName() { return "fire"; }
	virtual ColorValueHolder getColor() const override { return FireNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.5f, 0.0f); } //orange

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	FireNamedPower();
	static Power* factory();
};



class FireNamedTankPower : public TankPower {
protected:
	static const double bulletAngleDeviation;
	static const int bulletAmount;

public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return FireNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new FireNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint) override;
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	//virtual double getTankMaxSpeedMultiplier() const override { return .75; }
	virtual double getTankFiringRateMultiplier() const override { return .5; }
	virtual double getTankTurningIncrementMultiplier() const override { return 2; }

	FireNamedTankPower();
};



class FireNamedBulletPower : public BulletPower {
protected:
	static const double maxBulletAcceleration;
	static const double minBulletAcceleration;
	static const double degradeAmount;
	static const double growAmount;
	double accelerationAmount;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return FireNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override;
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() const override { return .5; }
	virtual double getBulletAcceleration() const override;

	FireNamedBulletPower();
	FireNamedBulletPower(double acceleration); //protected?
};
