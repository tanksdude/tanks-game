#pragma once
#include "../power.h"

class DevPusherPower : public Power {
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

	virtual std::string getName() const override { return DevPusherPower::getClassName(); }
	static std::string getClassName() { return "pusher"; }
	virtual ColorValueHolder getColor() const override { return DevPusherPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xBF/255.0, 0xBF/255.0, 0x30/255.0); } //big construction vehicle yellow (kinda)

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	DevPusherPower();
	static Power* factory();
};



class DevPusherTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return DevPusherPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new DevPusherTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Tank*, Wall*) override;

	virtual bool getModifiesCollisionWithCircleHazard(const CircleHazard* ch) const override;
	virtual InteractionBoolHolder modifiedCollisionWithCircleHazard(Tank*, CircleHazard*) override;

	virtual bool getModifiesCollisionWithRectHazard(const RectHazard* rh) const override;
	virtual InteractionBoolHolder modifiedCollisionWithRectHazard(Tank*, RectHazard*) override;

	DevPusherTankPower();
};



class DevPusherBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override { return DevPusherPower::getClassColor(); }

	virtual BulletPower* makeDuplicate() const override { return new DevPusherBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	DevPusherBulletPower();
};
