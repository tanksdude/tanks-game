#pragma once
#include "bounce-power.h"

class UltraBouncePower : public BouncePower {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return UltraBouncePower::getClassName(); }
	static std::string getClassName() { return "ultrabounce"; }
	virtual ColorValueHolder getColor() const override { return UltraBouncePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xE1/255.0, 0x53/255.0, 0xDB/255.0); } //pink but kinda white and purple

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	UltraBouncePower();
	static Power* factory();
};



class UltraBounceTankPower : public BounceTankPower {
public:
	virtual ColorValueHolder getColor() const override {
		return UltraBouncePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new UltraBounceTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	UltraBounceTankPower();
};



class UltraBounceBulletPower : public BounceBulletPower {
public:
	virtual ColorValueHolder getColor() const override {
		return UltraBouncePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override;
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;

	//bool modifiesCollisionWithEdge = true;
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) override;

	virtual double getBulletSpeedMultiplier() const override { return .25; }

	UltraBounceBulletPower();
	UltraBounceBulletPower(int bounces);
};
