#include "edge-sparks-power.h"

std::unordered_map<std::string, float> EdgeSparksPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

TankPower* EdgeSparksPower::makeTankPower() const {
	return new EdgeSparksTankPower();
}

BulletPower* EdgeSparksPower::makeBulletPower() const {
	return new EdgeSparksBulletPower();
}

/*
HazardPower* EdgeSparksPower::makeHazardPower() const {
	return new EdgeSparksHazardPower();
}
*/

Power* EdgeSparksPower::factory() {
	return new EdgeSparksPower();
}

EdgeSparksPower::EdgeSparksPower() : WallSparksPower() {
	return;
}



BulletPower* EdgeSparksTankPower::makeBulletPower() const {
	return new EdgeSparksBulletPower();
}

EdgeSparksTankPower::EdgeSparksTankPower() : WallSparksTankPower() {
	//nothing new
}



TankPower* EdgeSparksBulletPower::makeTankPower() const {
	return new EdgeSparksTankPower();
}

EdgeSparksBulletPower::EdgeSparksBulletPower() : EdgeSparksBulletPower(WallSparksPower::maxBounces) {}

EdgeSparksBulletPower::EdgeSparksBulletPower(int bounces) : WallSparksBulletPower(bounces) {
	//timeLeft = 0;
	//maxTime = -1;

	//bouncesLeft = bounces;
	if (bounces > 0) {
		modifiesEdgeCollision = true;
	}
	
	modifiesCollisionWithWall = false; //unset
}
