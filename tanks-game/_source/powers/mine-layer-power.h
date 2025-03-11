#pragma once
#include "../power.h"

class MineLayerPower : public Power {
public: //bullet stuff
	static const double tickCycle;

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

	virtual std::string getName() const override { return MineLayerPower::getClassName(); }
	static std::string getClassName() { return "mine_layer"; }
	virtual ColorValueHolder getColor() const override { return MineLayerPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.25f, 0.25f, 0.25f); } //dark gray

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	MineLayerPower();
	static Power* factory();
};



class MineLayerTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return MineLayerPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new MineLayerTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankFiringRateMultiplier() const override { return .5; }

	MineLayerTankPower();
};



class MineLayerBulletPower : public BulletPower {
protected:
	double tickCount;
	double last_xpos;
	double last_ypos;

	virtual void layMine(const Bullet*);

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override { return MineLayerPower::getClassColor(); }

	virtual BulletPower* makeDuplicate() const override { return new MineLayerBulletPower(); } //don't pass on tickCount
	virtual TankPower* makeTankPower() const override;

	//bool modifiesMovement = true;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) override;

	virtual double getBulletSpeedMultiplier() const override { return .5; }

	MineLayerBulletPower();
};
