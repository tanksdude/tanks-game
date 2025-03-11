#pragma once
#include "../power.h"

class DevOtherStuffIsPoisonPower : public Power {
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

	virtual std::string getName() const override { return DevOtherStuffIsPoisonPower::getClassName(); }
	static std::string getClassName() { return "other_stuff_is_poison"; }
	virtual ColorValueHolder getColor() const override { return DevOtherStuffIsPoisonPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xA3/255.0, 0xD6/255.0, 0x3E/255.0); } //greenish-yellow

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	DevOtherStuffIsPoisonPower();
	static Power* factory();
};



class DevOtherStuffIsPoisonTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return DevOtherStuffIsPoisonPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new DevOtherStuffIsPoisonTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesEdgeCollision = true; //shouldn't go that far
	//virtual InteractionBoolHolder modifiedEdgeCollision(Tank*) override { return { true }; }
	//bool overridesEdgeCollision = false;

	//bool modifiesCollisionWithTank = true; //maybe...
	//virtual InteractionBoolHolder modifiedCollisionWithTank(Tank* parent, Tank* other) override { return { true, false }; }
	//bool overridesCollisionWithTank = false;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Tank*, Wall*) override { return { true, false }; }
	//bool overridesCollisionWithWall = false;

	virtual bool getModifiesCollisionWithCircleHazard(const CircleHazard*) const override { return true; }
	virtual InteractionBoolHolder modifiedCollisionWithCircleHazard(Tank*, CircleHazard*) override { return { true, false }; }
	//bool overridesCollisionWithCircleHazard = false;

	virtual bool getModifiesCollisionWithRectHazard(const RectHazard*) const override { return true; }
	virtual InteractionBoolHolder modifiedCollisionWithRectHazard(Tank*, RectHazard*) override { return { true, false }; }
	//bool overridesCollisionWithRectHazard = false;

	virtual double getTankAccelerationMultiplier() const override { return .5; }

	DevOtherStuffIsPoisonTankPower();
};



class DevOtherStuffIsPoisonBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override { return DevOtherStuffIsPoisonPower::getClassColor(); }

	virtual BulletPower* makeDuplicate() const override { return new DevOtherStuffIsPoisonBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	DevOtherStuffIsPoisonBulletPower();
};
