#include "ultra-bounce-power.h"

//const int UltraBouncePower::maxBounces = 16;

std::unordered_map<std::string, float> UltraBouncePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", .5f });
	weights.insert({ "random-dev", .5f });
	return weights;
}

//process of narrowing down the specific color:
/*
//static ColorValueHolder getClassColor() { return ColorValueHolder(0.75f, 0.375f, 0.875f); } //bluish pink (but not really) (too close to homing)
//static ColorValueHolder getClassColor() { return ColorValueHolder(0xCA/255.0, 0x6B/255.0, 0xE3/255.0); } //basically that ^^^
//static ColorValueHolder getClassColor() { return ColorValueHolder(0xBA/255.0, 0x63/255.0, 0xED/255.0); } //pinkish purple
//static ColorValueHolder getClassColor() { return ColorValueHolder(0xB7/255.0, 0x4A/255.0, 0xF8/255.0); } //more purple
//static ColorValueHolder getClassColor() { return ColorValueHolder(0x88/255.0, 0x55/255.0, 0xE3/255.0); } //homing then hueshifted blue, mostly
//static ColorValueHolder getClassColor() { return ColorValueHolder(0xA6/255.0, 0x2A/255.0, 0xB7/255.0); } //homing then not faded
//static ColorValueHolder getClassColor() { return ColorValueHolder(0xB8/255.0, 0x28/255.0, 0xC3/255.0); } //^^^ then more pink (awful)
//static ColorValueHolder getClassColor() { return ColorValueHolder(0xE7/255.0, 0x72/255.0, 0xE2/255.0); } //pink then lighter (good)
//static ColorValueHolder getClassColor() { return ColorValueHolder(0xFB/255.0, 0x6F/255.0, 0xF5/255.0); } //^^^ then more pink (too close to bounce + invincible)
static ColorValueHolder getClassColor() { return ColorValueHolder(0xE1/255.0, 0x53/255.0, 0xDB/255.0); } //more pink again (not sure it's good, but whatever)
*/

TankPower* UltraBouncePower::makeTankPower() const {
	return new UltraBounceTankPower();
}

BulletPower* UltraBouncePower::makeBulletPower() const {
	return new UltraBounceBulletPower();
}

/*
HazardPower* UltraBouncePower::makeHazardPower() const {
	return new UltraBounceHazardPower();
}
*/

Power* UltraBouncePower::factory() {
	return new UltraBouncePower();
}

UltraBouncePower::UltraBouncePower() : BouncePower() {
	return;
}



void UltraBounceTankPower::initialize(Tank* parent) {
	//nothing
}

void UltraBounceTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* UltraBounceTankPower::makeBulletPower() const {
	return new UltraBounceBulletPower();
}

UltraBounceTankPower::UltraBounceTankPower() : BounceTankPower() {
	/*
	maxTime = 500;
	timeLeft = 500;
	*/
}



#include "collision-handler.h"
#include "power-function-helper.h"

InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> UltraBounceBulletPower::modifiedCollisionWithWall(const Bullet* b, const Wall* w) {
	std::shared_ptr<BulletUpdateStruct> b_update;
	std::shared_ptr<WallUpdateStruct> w_update;

	//if (abs(b->velocity.getMagnitude()) * Bullet::default_radius/b->r <= .5) {
	if (true) { //it's a dev power, so may as well
		auto result = PowerFunctionHelper::superbounceGenericWithCorners(b, w, b->r/2);
		b_update = result.second.firstUpdate;
		w_update = result.second.secondUpdate;
		if (result.first) {
			bouncesLeft--;
		}
	} else {
		auto result = PowerFunctionHelper::superbounceGeneric(b, w, b->r/2);
		b_update = result.second.firstUpdate;
		w_update = result.second.secondUpdate;
		if (result.first) {
			bouncesLeft--;
		}
	}
	//makes more sense for the strength to be dependent on velocity...

	if (bouncesLeft <= 0) {
		modifiesCollisionWithWall = false;
		modifiesEdgeCollision = false;
	}

	return { (bouncesLeft < 0), false, b_update, w_update };
}

InteractionBoolHolder UltraBounceBulletPower::modifiedEdgeCollision(Bullet* b) {
	//match with BounceBulletPower::modifiedEdgeCollision

	bool bouncedY = false;
	//bool bouncedX = false;
	BulletUpdateStruct b_update;

	if (CollisionHandler::partiallyOutOfBounds(b)) {
		auto result = PowerFunctionHelper::superbounceEdgeGenericY(b, b->r/2);
		if (result.first) {
			bouncesLeft--;
			bouncedY = true;
		}
		//TODO: update modifiedEdgeCollision to also use update structs
		b->update(&result.second);
		if (bouncesLeft <= 0) {
			modifiesCollisionWithWall = false;
			modifiesEdgeCollision = false;
			return { CollisionHandler::fullyOutOfBounds(b) };
		}
	}

	if (CollisionHandler::partiallyOutOfBounds(b)) {
		auto result = PowerFunctionHelper::superbounceEdgeGenericX(b, b->r/2);
		if (result.first) {
			bouncesLeft--;
			//bouncedX = true;
		}
		//TODO: update modifiedEdgeCollision to also use update structs
		b->update(&result.second);
		if (bouncesLeft <= 0) {
			modifiesCollisionWithWall = false;
			modifiesEdgeCollision = false;
			return { CollisionHandler::fullyOutOfBounds(b) };
		}
	}

	if (!bouncedY && CollisionHandler::partiallyOutOfBounds(b)) {
		auto result = PowerFunctionHelper::superbounceEdgeGenericY(b, b->r/2);
		if (result.first) {
			bouncesLeft--;
			//bouncedY = true;
		}
		//TODO: update modifiedEdgeCollision to also use update structs
		b->update(&result.second);
		if (bouncesLeft <= 0) {
			modifiesCollisionWithWall = false;
			modifiesEdgeCollision = false;
			//return { CollisionHandler::fullyOutOfBounds(b) };
		}
	}

	return { CollisionHandler::fullyOutOfBounds(b) };
}

void UltraBounceBulletPower::initialize(Bullet* parent) {
	//nothing
}

void UltraBounceBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

BulletPower* UltraBounceBulletPower::makeDuplicate() const {
	return new UltraBounceBulletPower(this->bouncesLeft);
}

TankPower* UltraBounceBulletPower::makeTankPower() const {
	return new UltraBounceTankPower();
}

UltraBounceBulletPower::UltraBounceBulletPower() : UltraBounceBulletPower(BouncePower::maxBounces) {}

UltraBounceBulletPower::UltraBounceBulletPower(int bounces) : BounceBulletPower(bounces) {
	/*
	timeLeft = 0;
	maxTime = -1;

	this->bouncesLeft = bounces;
	if (bounces > 0) {
		modifiesCollisionWithWall = true;
		//modifiedCollisionWithWallCanWorkWithOthers = false;
		modifiesEdgeCollision = true;
	}
	*/
}
