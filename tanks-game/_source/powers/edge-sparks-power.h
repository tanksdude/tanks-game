#pragma once
#include "wall-sparks-power.h"

class EdgeSparksPower : public WallSparksPower {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return EdgeSparksPower::getClassName(); }
	static std::string getClassName() { return "edge_sparks"; }
	virtual ColorValueHolder getColor() const override { return EdgeSparksPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return WallSparksPower::getClassColor(); } //same for now

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	EdgeSparksPower();
	static Power* factory();
};



class EdgeSparksTankPower : public WallSparksTankPower {
public:
	virtual ColorValueHolder getColor() const override { return EdgeSparksPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new EdgeSparksTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	EdgeSparksTankPower();
};



class EdgeSparksBulletPower : public WallSparksBulletPower {
public:
	virtual ColorValueHolder getColor() const override { return EdgeSparksPower::getClassColor(); }
	virtual std::string getColorIdentifier() const override { return EdgeSparksPower::getClassName(); }

	virtual BulletPower* makeDuplicate() const override { return new EdgeSparksBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//bool modifiesEdgeCollision = true;

	//bool modifiesCollisionWithWall = false;

	EdgeSparksBulletPower();
	EdgeSparksBulletPower(int bounces);
};
