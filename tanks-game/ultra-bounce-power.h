#pragma once
//#include "power.h"
#include "bounce-power.h"

class UltraBouncePower : public BouncePower {
public: //bullet stuff
	//static const int maxBounces;

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	//virtual std::vector<std::string> getPowerAttributes() const override;

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
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return UltraBouncePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new UltraBounceTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	UltraBounceTankPower();
};



class UltraBounceBulletPower : public BounceBulletPower {
protected:
	//int bouncesLeft;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return UltraBouncePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override;
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;

	//bool modifiesCollisionWithEdge = true;
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) override;

	virtual double getBulletSpeedMultiplier() const override { return .25; }

	UltraBounceBulletPower();
	UltraBounceBulletPower(int bounces);
};
