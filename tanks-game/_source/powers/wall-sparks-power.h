#pragma once
#include "../power.h"

class WallSparksPower : public Power {
public: //bullet stuff
	static const int extraBulletsCount;
	static const int maxBounces;
	static const double maxNewBulletVelocity;
	static const double minNewBulletVelocity;
	static const double bulletAngleDeviation;

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		//shouldn't stack because it bounces
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return WallSparksPower::getClassName(); }
	static std::string getClassName() { return "wall_sparks"; }
	virtual ColorValueHolder getColor() const override { return WallSparksPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xBC/255.0, 0x38/255.0, 0x28/255.0); } //blood orange

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	WallSparksPower();
	static Power* factory();
};



class WallSparksTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override {
		return WallSparksPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new WallSparksTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	WallSparksTankPower();
};



class WallSparksBulletPower : public BulletPower {
protected:
	int bouncesLeft;

	virtual void sparkExplode(const Bullet*, const BulletUpdateStruct*);

public:
	virtual ColorValueHolder getColor() const override {
		return WallSparksPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new WallSparksBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) override;

	//bool modifiesCollisionWithEdge = false; //important: not intended for this power; defined so edge sparks can use it
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) override;

	WallSparksBulletPower();
	WallSparksBulletPower(int bounces);
};
