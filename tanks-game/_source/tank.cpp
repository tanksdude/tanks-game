#include "tank.h"

#include "constants.h"
#include <cmath>
#include <algorithm> //std::sort, std::clamp, std::count_if
#include <iostream>
#include "rng.h" //just for shiny tanks

#include "renderer.h"
#include "color-mixer.h"
#include "background-rect.h"

#include "game-manager.h" //INI file
#include "bullet-manager.h"
#include "level-manager.h"

/*
struct TankInputChar {
protected:
	std::string key;
	bool isSpecial;
	int key_num;
public:
	std::string getKey() const { return key; }
	bool getKeyState() const;
	TankInputChar(std::string);
	//TankInputChar(bool, int);
	TankInputChar();
};

TankInputChar::TankInputChar(std::string key_input) {
	key = key_input;
	if (KeypressManager::keyIsSpecialFromString(key_input)) {
		isSpecial = true;
		key_num = KeypressManager::specialKeyFromString(key_input);
	} else {
		isSpecial = false;
		key_num = KeypressManager::normalKeyFromString(key_input);
	}
}
TankInputChar::TankInputChar() {
	key = "`";
	isSpecial = false;
	key_num = '`';
}

bool TankInputChar::getKeyState() const {
	if (isSpecial) {
		return KeypressManager::getSpecialKey(key_num);
	}
	return KeypressManager::getNormalKey(key_num);
}
*/

SimpleVector2D Tank::body_vertices[Circle::numOfSides+1];
unsigned int Tank::body_indices[Circle::numOfSides*3];
unsigned int Tank::outline_indices[Circle::numOfSides*2*3];
bool Tank::initialized_vertices = false;

const double Tank::default_maxSpeed = 1;
const double Tank::default_acceleration = 1.0/16;
const double Tank::default_turningIncrement = 64;

Tank::Tank(double x_, double y_, double angle, Team_ID teamID, std::string name_, double shootCooldown) : GameThing(teamID) {
	x = x_;
	y = y_;
	velocity = SimpleVector2D(angle, 0, true);
	r = TANK_RADIUS;
	name = name_;
	this->dead = false;

	maxSpeed = default_maxSpeed;
	acceleration = default_acceleration;
	turningIncrement = default_turningIncrement;

	shootCount = 0;
	maxShootCount = shootCooldown;

	shootingPoints = std::vector<CannonPoint>();
	extraShootingPoints = std::vector<ExtraCannonPoint>();
	determineShootingAngles();
	updateAllValues();

	if (RNG::randFunc() < 1.0/4096) {
		//shiny tank (yes, 1/8192 is the chance before Sword/Shield)
		defaultColor = ColorValueHolder(0.75f, 0.75f, 0.75f);
	} else {
		defaultColor = ColorValueHolder(0.5f, 0.5f, 0.5f);
	}

	initializeVertices();
}

Tank::~Tank() {
	for (int i = 0; i < tankPowers.size(); i++) {
		delete tankPowers[i];
	}
	//tankPowers.clear();
}

bool Tank::initializeVertices() {
	if (initialized_vertices) { [[likely]]
		return false;
	}

	body_vertices[0] = SimpleVector2D(0, 0);
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		body_vertices[i] = SimpleVector2D(cos((i-1) * (2*PI / Circle::numOfSides)), sin((i-1) * (2*PI / Circle::numOfSides)));
	}

	for (int i = 0; i < Circle::numOfSides; i++) {
		body_indices[i*3]   = 0;
		body_indices[i*3+1] = i+1;
		body_indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	for (int i = 0; i < Circle::numOfSides; i++) {
		outline_indices[i*6]   =  i*2;
		outline_indices[i*6+1] =  i*2+1;
		outline_indices[i*6+2] = (i*2+3) % (Circle::numOfSides*2);
		outline_indices[i*6+3] = (i*2+3) % (Circle::numOfSides*2);
		outline_indices[i*6+4] = (i*2+2) % (Circle::numOfSides*2);
		outline_indices[i*6+5] =  i*2;
	}

	initialized_vertices = true;
	return true;
}

bool Tank::move(bool forward, bool turnL, bool turnR, bool specialKey) {
	bool shouldBeKilled = false;
	bool modifiedMovement = false;
	bool overridedMovement = false;
	bool noMoreMovementSpecials = false;
	//TODO: handle killing the tankpowers

	for (int k = 0; k < tankPowers.size(); k++) {
		if (tankPowers[k]->modifiesMovement) {
			if (tankPowers[k]->modifiedMovementCanOnlyWorkIndividually && modifiedMovement) {
				continue;
			}
			if (noMoreMovementSpecials) {
				continue;
			}

			modifiedMovement = true;
			if (tankPowers[k]->overridesMovement) {
				overridedMovement = true;
			}
			if (!tankPowers[k]->modifiedMovementCanWorkWithOthers) {
				noMoreMovementSpecials = true;
			}

			InteractionBoolHolder check_temp = tankPowers[k]->modifiedMovement(this, forward, turnL, turnR, specialKey);
			if (check_temp.shouldDie) {
				shouldBeKilled = true;
				overridedMovement = true;
				break;
			}
		}
	}

	if (!overridedMovement) {
		move_base(forward, turnL, turnR);
	}

	return shouldBeKilled;
}

inline void Tank::move_base(bool forward, bool turnL, bool turnR) {
	const BasicINIParser::BasicINIData& ini_data = GameManager::get_INI();

	if (ini_data.exists("GAME_OPTIONS", "RestrictTankTurning") && std::stoi(ini_data.get("GAME_OPTIONS", "RestrictTankTurning"))) {
		if (!forward) {
			if (turnL) {
				velocity.changeAngle(PI/turningIncrement);
			}
			if (turnR) {
				velocity.changeAngle(-PI/turningIncrement);
			}
		}
	} else { [[likely]]
		if (turnL) {
			velocity.changeAngle(PI/turningIncrement);
		}
		if (turnR) {
			velocity.changeAngle(-PI/turningIncrement);
		}
	}

	if (ini_data.exists("GAME_OPTIONS", "NoTankAcceleration") && std::stoi(ini_data.get("GAME_OPTIONS", "NoTankAcceleration"))) {
		if (forward) {
			velocity.setMagnitude(maxSpeed);
		} else {
			velocity.setMagnitude(0);
		}
	} else { [[likely]]
		if (forward) {
			velocity.changeMagnitude(acceleration);
		} else {
			velocity.changeMagnitude(-acceleration); //can result in negative velocities, fixed by SimpleVector2D just not allowing that
		}
		terminalVelocity(forward);
	}

	x += velocity.getXComp();
	y += velocity.getYComp();
}

inline void Tank::terminalVelocity(bool forward) {
	if (velocity.getMagnitude() > maxSpeed + acceleration) {
		velocity.changeMagnitude(-acceleration);
		if (forward && velocity.getMagnitude() > maxSpeed) {
			//so the tank doesn't stay at a high velocity if it loses its ability to go as fast as it previously could (maybe change?)
			velocity.changeMagnitude(-acceleration);
		}
	} else if (velocity.getMagnitude() > maxSpeed) {
		velocity.setMagnitude(maxSpeed);
	}
}

void Tank::shoot(bool shooting) {
	if (shootCount > 0) { //check isn't really needed, but it also doesn't decrease performance by a real amount
		shootCount--;
	}

	if (shooting && shootCount <= 0) {
		const BasicINIParser::BasicINIData& ini_data = GameManager::get_INI();

		//determineShootingAngles(); //TODO: is this needed?
		bool modifiedAdditionalShooting = false;
		bool overridedShooting = false;
		bool noMoreOtherAdditionalShooting = false;

		for (int j = 0; j < tankPowers.size(); j++) {
			if (tankPowers[j]->modifiesAdditionalShooting) {
				if (tankPowers[j]->additionalShootingCanOnlyWorkIndividually && modifiedAdditionalShooting) {
					continue;
				}
				if (noMoreOtherAdditionalShooting) {
					continue;
				}
				modifiedAdditionalShooting = true;
				if (tankPowers[j]->overridesAdditionalShooting) {
					overridedShooting = true;
				}
				if (!tankPowers[j]->additionalShootingCanWorkWithOthers) {
					noMoreOtherAdditionalShooting = true;
				}

				if (ini_data.exists("GAME_OPTIONS", "FewerExtraShootingBullets") && !std::stoi(ini_data.get("GAME_OPTIONS", "FewerExtraShootingBullets"))) {
					//more
					for (int i = shootingPoints.size() - 1; i >= 0; i--) {
						for (int s2 = extraShootingPoints.size() - 1; s2 >= 0; s2--) {
							tankPowers[j]->additionalShooting(this, shootingPoints.at(i), extraShootingPoints.at(s2));
						}
					}
				} else {
					//fewer
					for (int i = shootingPoints.size() - 1; i >= 0; i--) {
						for (int s2 = extraShootingPoints.size() - 1; s2 >= 1; s2--) {
							defaultMakeBullet(velocity.getAngle() + shootingPoints[i].angleFromCenter + extraShootingPoints[s2].angleFromCenter, extraShootingPoints[s2].angleFromEdge);
						}
						tankPowers[j]->additionalShooting(this, shootingPoints.at(i), extraShootingPoints.at(0));
					}
				}
			}
		}

		if (!overridedShooting) {
			for (int i = shootingPoints.size() - 1; i >= 0; i--) {
				for (int s2 = extraShootingPoints.size() - 1; s2 >= 0; s2--) {
					defaultMakeBullet(velocity.getAngle() + shootingPoints[i].angleFromCenter + extraShootingPoints[s2].angleFromCenter, extraShootingPoints[s2].angleFromEdge);
				}
			}
		}

		shootCount = maxShootCount * getShootingSpeedMultiplier();
	}
}

void Tank::makeBulletCommon(double x, double y, double angle, double radius, double speed) {
	std::vector<BulletPower*>* bp = new std::vector<BulletPower*>;
	bp->reserve(tankPowers.size());
	for (int k = 0; k < tankPowers.size(); k++) {
		bp->push_back(tankPowers[k]->makeBulletPower());
	}

	Bullet* temp = new Bullet(x, y, radius, angle, speed, getTeamID(), BulletParentType::team, getGameID(), bp);
	BulletManager::pushBullet(temp);

	delete bp;
}

void Tank::makeBullet(double x, double y, double angle, double radius, double speed, double acc) {
	std::vector<BulletPower*>* bp = new std::vector<BulletPower*>;
	bp->reserve(tankPowers.size());
	for (int k = 0; k < tankPowers.size(); k++) {
		bp->push_back(tankPowers[k]->makeBulletPower());
	}

	Bullet* temp = new Bullet(x, y, radius, angle, speed, acc, getTeamID(), BulletParentType::team, getGameID(), bp, true);
	BulletManager::pushBullet(temp);

	delete bp;
	//bp can be deleted here or in the Bullet constructor, since every bullet should get a different bulletpower vector
}

void Tank::defaultMakeBullet(double angle) {
	makeBulletCommon(x + r*cos(angle), y + r*sin(angle), angle, r*BULLET_TO_TANK_RADIUS_RATIO, maxSpeed*BULLET_TO_TANK_SPEED_RATIO);
}

void Tank::defaultMakeBullet(double angle, double edgeAngleOffset) {
	makeBulletCommon(x + r*cos(angle), y + r*sin(angle), angle + edgeAngleOffset, r*BULLET_TO_TANK_RADIUS_RATIO, maxSpeed*BULLET_TO_TANK_SPEED_RATIO);
}

void Tank::preciseMakeBullet(double x, double y, double angle) {
	makeBulletCommon(this->x + x, this->y + y, angle, r*BULLET_TO_TANK_RADIUS_RATIO, maxSpeed*BULLET_TO_TANK_SPEED_RATIO);
}

void Tank::determineShootingAngles() {
	shootingPoints.clear();
	shootingPoints.push_back(CannonPoint(0));
	extraShootingPoints.clear();
	extraShootingPoints.push_back(ExtraCannonPoint(0, 0));

	bool modifiedAddsShootingPoints = false;
	bool noMoreAddsShootingPointsSpecials = false;

	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->addsShootingPoints) {
			if (tankPowers[i]->addShootingPointsCanOnlyWorkIndividually && modifiedAddsShootingPoints) {
				continue;
			}
			if (noMoreAddsShootingPointsSpecials) {
				continue;
			}

			modifiedAddsShootingPoints = true;
			if (!tankPowers[i]->addExtraShootingPointsCanWorkWithOthers) {
				noMoreAddsShootingPointsSpecials = true;
			}

			determineShootingAngles_helper(tankPowers[i]->addShootingPoints());
		}
	}

	bool modifiedAddsExtraShootingPoints = false;
	bool noMoreAddsExtraShootingPointsSpecials = false;

	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->addsExtraShootingPoints) {
			if (tankPowers[i]->addExtraShootingPointsCanOnlyWorkIndividually && modifiedAddsExtraShootingPoints) {
				continue;
			}
			if (noMoreAddsExtraShootingPointsSpecials) {
				continue;
			}

			modifiedAddsExtraShootingPoints = true;
			if (!tankPowers[i]->addExtraShootingPointsCanWorkWithOthers) {
				noMoreAddsExtraShootingPointsSpecials = true;
			}

			std::vector<std::pair<double, double>>* extraCannons = tankPowers[i]->addExtraShootingPoints();
			for (int j = 0; j < extraCannons->size(); j++) {
				extraShootingPoints.push_back({ extraCannons->at(j).first, extraCannons->at(j).second });
			}
			delete extraCannons;
		}
	}
}

inline void Tank::determineShootingAngles_helper(std::vector<double>* newCannonPoints) {
	for (int i = shootingPoints.size() - 1; i >= 0; i--) {
		const int end = (i + 1) % shootingPoints.size();
		const double angle_diff = end == 0 ?
		                          2*PI - (shootingPoints[i].angleFromCenter - shootingPoints[end].angleFromCenter) :
		                          shootingPoints[end].angleFromCenter - shootingPoints[i].angleFromCenter;

		for (int j = 0; j < newCannonPoints->size(); j++) {
			const double newAngle = angle_diff * newCannonPoints->at(j);
			CannonPoint temp = CannonPoint(newAngle + shootingPoints[i].angleFromCenter);
			shootingPoints.insert(shootingPoints.begin() + i + j + 1, temp);
		}
	}

	delete newCannonPoints;
}

double Tank::getShootingSpeedMultiplier() const {
	//so this function will look at the firing rate multipliers provided by the tankpowers
	//(0-1] range: use lowest; (1-inf) range: use highest
	//if there are values in each range, then there are three options:
	//1. return either lowest or highest; 2. return average of lowest and highest; 3. return lowest * highest
	//1 is dumb, 2 would probably be fine if there aren't *8 or /8 multipliers, 3 will probably be what I'll use (they will typically be recipricols, so it makes sense)

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList; //only now have I realized this isn't a great name; no, it's not a data structure known as a stack

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getTankFiringRateMultiplier();
		if (tankPowers[i]->tankFiringRateStacks) {
			stackList.push_back(value);
		} else {
			if (value < 1 && value < lowest) {
				lowest = value;
			} else if (value > 1 && value > highest) {
				highest = value;
			}
		}
		//technically don't need to check value against 1, now do I?
	}

	double value = 1;
	for (int i = 0; i < stackList.size(); i++) {
		value *= stackList[i];
	}

	double level_amount = 1;
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			LevelEffect* le = l->getLevelEffect(j);
			level_amount *= le->getTankFiringRateMultiplier();
		}
	}

	return highest * lowest * value * level_amount; //unintentionally works out cleanly
}

void Tank::updateAllValues() {
	updateMaxSpeed();
	updateAcceleration();
	updateRadius();
	updateTurningIncrement();
}

/*
// C++ really sucks sometimes
// this was going to make the copy & pasted code so much cleaner, but C++ gotta be like C sometimes
typedef double (TankPower::*memberFuncPointer)(void);
void Tank::updateSpecificValue(double& attribute, double (TankPower::*func)(void), bool (TankPower::*b), double multiplier) {
	//look at getShootingSpeedMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	memberFuncPointer p = &TankPower::getTankMaxSpeedMultiplier;

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = func();
		if (tankPowers[i]->tankMaxSpeedStacks) {
			stackList.push_back(value);
		} else {
			if (value < 1 && value < lowest) {
				lowest = value;
			} else if (value > 1 && value > highest) {
				highest = value;
			}
		}
	}

	double value = 1;
	for (int i = 0; i < stackList.size(); i++) {
		value *= stackList[i];
	}

	//[insert level effect value getting here]

	attribute = highest * lowest * value * multiplier;
}
*/

void Tank::updateMaxSpeed() {
	//look at getShootingSpeedMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getTankMaxSpeedMultiplier();
		if (tankPowers[i]->tankMaxSpeedStacks) {
			stackList.push_back(value);
		} else {
			if (value < 1 && value < lowest) {
				lowest = value;
			} else if (value > 1 && value > highest) {
				highest = value;
			}
		}
	}

	double value = 1;
	for (int i = 0; i < stackList.size(); i++) {
		value *= stackList[i];
	}

	double level_amount = 1;
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			LevelEffect* le = l->getLevelEffect(j);
			level_amount *= le->getTankMaxSpeedMultiplier();
		}
	}

	maxSpeed = highest * lowest * value * level_amount * default_maxSpeed;
}

void Tank::updateAcceleration() {
	//look at getShootingSpeedMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getTankAccelerationMultiplier();
		if (tankPowers[i]->tankAccelerationStacks) {
			stackList.push_back(value);
		} else {
			if (value < 1 && value < lowest) {
				lowest = value;
			} else if (value > 1 && value > highest) {
				highest = value;
			}
		}
	}

	double value = 1;
	for (int i = 0; i < stackList.size(); i++) {
		value *= stackList[i];
	}

	double level_amount = 1;
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			LevelEffect* le = l->getLevelEffect(j);
			level_amount *= le->getTankAccelerationMultiplier();
		}
	}

	acceleration = highest * lowest * value * level_amount * default_acceleration;
}

void Tank::updateRadius() {
	//look at getShootingSpeedMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getTankRadiusMultiplier();
		if (tankPowers[i]->tankRadiusStacks) {
			stackList.push_back(value);
		} else {
			if (value < 1 && value < lowest) {
				lowest = value;
			} else if (value > 1 && value > highest) {
				highest = value;
			}
		}
	}

	double value = 1;
	for (int i = 0; i < stackList.size(); i++) {
		value *= stackList[i];
	}

	double level_amount = 1;
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			LevelEffect* le = l->getLevelEffect(j);
			level_amount *= le->getTankRadiusMultiplier();
		}
	}

	r = highest * lowest * value * level_amount * TANK_RADIUS;
}

void Tank::updateTurningIncrement() {
	//look at getShootingSpeedMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;
	int negativeCount = 0;

	for (int i = 0; i < tankPowers.size(); i++) {
		double value = tankPowers[i]->getTankTurningIncrementMultiplier();
		if (value < 0) {
			negativeCount++;
			value = value * -1;
		}
		if (tankPowers[i]->tankTurningIncrementStacks) {
			stackList.push_back(value);
		} else {
			if (value < 1 && value < lowest) {
				lowest = value;
			} else if (value > 1 && value > highest) {
				highest = value;
			}
		}
	}

	double value = 1;
	for (int i = 0; i < stackList.size(); i++) {
		value *= stackList[i];
	}

	double level_amount = 1;
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			LevelEffect* le = l->getLevelEffect(j);
			level_amount *= le->getTankTurningIncrementMultiplier();
		}
	}

	turningIncrement = highest * lowest * value * level_amount * default_turningIncrement * (negativeCount%2 == 0 ? 1 : -1);
	velocity.setAngle(round(velocity.getAngle() / (PI / turningIncrement)) * (PI / turningIncrement));
}

void Tank::powerCalculate() {
	//TODO: separate into power tick and power count down
	for (int i = tankPowers.size() - 1; i >= 0; i--) {
		tankPowers[i]->tick(this); //I don't think any power will use this, but whatever
		if (tankPowers[i]->isDone()) { [[unlikely]]
			removePower(i);
		} else { //to make each power last its full length, not n-1 length
			tankPowers[i]->powerTick();
		}
	}
}

void Tank::removePower(int index) {
	tankPowers[index]->removeEffects(this);
	delete tankPowers[index];
	tankPowers.erase(tankPowers.begin() + index);
	determineShootingAngles();
	updateAllValues();
}

void Tank::powerReset() {
	for (int i = tankPowers.size() - 1; i >= 0; i--) {
		removePower(i);
	}
}

ColorValueHolder Tank::getBodyColor() const {
	if (tankPowers.size() == 0) {
		return defaultColor;
	} else {
		float highest = LOW_IMPORTANCE;
		for (int i = 0; i < tankPowers.size(); i++) {
			if (tankPowers[i]->getColorImportance() > highest) {
				highest = tankPowers[i]->getColorImportance();
			}
		}
		if (highest < 0) {
			return defaultColor;
		}
		std::vector<ColorValueHolder> mixingColors;
		for (int i = 0; i < tankPowers.size(); i++) {
			if (tankPowers[i]->getColorImportance() == highest) {
				mixingColors.push_back(tankPowers[i]->getColor());
			}
		}
		return ColorMixer::mix(mixingColors.data(), mixingColors.size());
	}
}

inline double Tank::getEvaluatedCannonAngle(unsigned int i) const {
	//return fmod(fmod(shootingPoints[i].angleFromCenter + velocity.getAngle(), 2*PI) + 2*PI, 2*PI);
	return velocity.getAngle() + shootingPoints[i].angleFromCenter;
}

inline double Tank::getEvaluatedCannonAngle(unsigned int i, unsigned int j) const {
	return velocity.getAngle() + shootingPoints[i].angleFromCenter + extraShootingPoints[j].angleFromCenter;
}

inline double Tank::getEvaluatedCannonAngleWithEdge(unsigned int i, unsigned int j) const {
	return velocity.getAngle() + shootingPoints[i].angleFromCenter + extraShootingPoints[j].angleFromCenter + extraShootingPoints[j].angleFromEdge;
}

void Tank::draw() const {
	if (this->dead) {
		drawDead();
	} else {
		drawShootingCooldown();
		drawPowerCooldown();
		drawBody();
		drawExtraBarrels();
		drawExtraExtraBarrels();
		drawOutline();
		drawMainBarrel();
	}
}

void Tank::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Tank::draw!" << std::endl;
		case DrawingLayers::normal:
			if (!this->dead) {
				drawShootingCooldown();
				drawPowerCooldown();
				drawBody();
				drawExtraBarrels();
				drawExtraExtraBarrels();
				drawOutline();
				drawMainBarrel();
			}
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			if (this->dead) {
				drawDead();
			}
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void Tank::poseDraw() const {
	//does not have to worry about being dead
	drawBody();
	//drawExtraBarrels();
	//drawExtraExtraBarrels();
	drawOutline();
	drawMainBarrel();
}

void Tank::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Tank::poseDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBody();
			//drawExtraBarrels();
			//drawExtraExtraBarrels();
			drawOutline();
			drawMainBarrel();
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//drawDead();
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void Tank::ghostDraw(float alpha) const {
	//does not have to worry about being dead
	drawBody(alpha);
	drawExtraBarrels(alpha);
	drawExtraExtraBarrels(alpha);
	drawOutline(alpha);
	drawMainBarrel(alpha);
}

void Tank::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Tank::ghostDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBody(alpha);
			drawExtraBarrels(alpha);
			drawExtraExtraBarrels(alpha);
			drawOutline(alpha);
			drawMainBarrel(alpha);
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//drawDead();
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

inline void Tank::drawBody(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	if (this->dead) {
		ColorValueHolder deadColor = ColorValueHolder(0.0f, 0.0f, 0.0f);
		ColorValueHolder outerColor = ColorValueHolder(1.0f, 1.0f, 1.0f);
		deadColor = ColorMixer::mix(BackgroundRect::getBackColor(), deadColor, alpha);
		outerColor = ColorMixer::mix(BackgroundRect::getBackColor(), outerColor, alpha);

		float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
		coordsAndColor[0] = x;
		coordsAndColor[1] = y;
		coordsAndColor[2] = deadColor.getRf();
		coordsAndColor[3] = deadColor.getGf();
		coordsAndColor[4] = deadColor.getBf();
		coordsAndColor[5] = deadColor.getAf();
		for (int i = 1; i < Circle::numOfSides+1; i++) {
			coordsAndColor[i*6]   = x + r * body_vertices[i].getXComp();
			coordsAndColor[i*6+1] = y + r * body_vertices[i].getYComp();
			coordsAndColor[i*6+2] = outerColor.getRf();
			coordsAndColor[i*6+3] = outerColor.getGf();
			coordsAndColor[i*6+4] = outerColor.getBf();
			coordsAndColor[i*6+5] = outerColor.getAf();
		}

		Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides+1)*(2+4), body_indices, Circle::numOfSides*3);
	} else {
		const int visiblePowerCount = std::count_if(tankPowers.begin(), tankPowers.end(),
			[](const TankPower* tp) { return (tp->getColorImportance() >= 0); });

		if (visiblePowerCount <= 1) {
			ColorValueHolder color = getBodyColor();
			color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

			float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
			coordsAndColor[0] = x;
			coordsAndColor[1] = y;
			coordsAndColor[2] = color.getRf();
			coordsAndColor[3] = color.getGf();
			coordsAndColor[4] = color.getBf();
			coordsAndColor[5] = color.getAf();
			for (int i = 1; i < Circle::numOfSides+1; i++) {
				coordsAndColor[i*6]   = x + r * body_vertices[i].getXComp();
				coordsAndColor[i*6+1] = y + r * body_vertices[i].getYComp();
				coordsAndColor[i*6+2] = color.getRf();
				coordsAndColor[i*6+3] = color.getGf();
				coordsAndColor[i*6+4] = color.getBf();
				coordsAndColor[i*6+5] = color.getAf();
			}

			Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides+1)*(2+4), body_indices, Circle::numOfSides*3);
		} else {
			//main color split:

			ColorValueHolder color;
			for (int i = 0; i < tankPowers.size(); i++) {
				if (tankPowers[i]->getColorImportance() < 0) {
					continue;
				}
				std::vector<float> coordsAndColor_colorSplit;
				std::vector<unsigned int> indices_colorSplit;
				//could just use an array, since the size should be easy to calculate, but that's effort
				//could push everything to an array then only submit one batched draw call, but that's more effort

				color = tankPowers[i]->getColor();
				color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

				float rotatePercent = floor((float(i) / visiblePowerCount) * Circle::numOfSides) / Circle::numOfSides;
				float nextRotatePercent = floor((float(i+1) / visiblePowerCount) * Circle::numOfSides) / Circle::numOfSides;
				//unsigned int rotateVertices = floor((nextRotatePercent - rotatePercent) * Circle::numOfSides);
				unsigned int rotateVertexStart = floor(rotatePercent * Circle::numOfSides);
				unsigned int rotateVertexEnd = floor(nextRotatePercent * Circle::numOfSides);

				coordsAndColor_colorSplit.push_back(x);
				coordsAndColor_colorSplit.push_back(y);
				coordsAndColor_colorSplit.push_back(color.getRf());
				coordsAndColor_colorSplit.push_back(color.getGf());
				coordsAndColor_colorSplit.push_back(color.getBf());
				coordsAndColor_colorSplit.push_back(color.getAf());
				for (unsigned int j = rotateVertexStart; j <= rotateVertexEnd; j++) {
					SimpleVector2D vertex = SimpleVector2D(body_vertices[j % Circle::numOfSides + 1]);
					vertex.scaleAndRotate(r, velocity.getAngle());

					coordsAndColor_colorSplit.push_back(x + vertex.getXComp());
					coordsAndColor_colorSplit.push_back(y + vertex.getYComp());
					coordsAndColor_colorSplit.push_back(color.getRf());
					coordsAndColor_colorSplit.push_back(color.getGf());
					coordsAndColor_colorSplit.push_back(color.getBf());
					coordsAndColor_colorSplit.push_back(color.getAf());
				}

				Renderer::SubmitBatchedDraw(coordsAndColor_colorSplit.data(), coordsAndColor_colorSplit.size(), body_indices, (rotateVertexEnd - rotateVertexStart)*3);
			}

			//center colors mix:

			color = getBodyColor();
			color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

			float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
			coordsAndColor[0] = x;
			coordsAndColor[1] = y;
			coordsAndColor[2] = color.getRf();
			coordsAndColor[3] = color.getGf();
			coordsAndColor[4] = color.getBf();
			coordsAndColor[5] = color.getAf();
			for (int i = 1; i < Circle::numOfSides+1; i++) {
				coordsAndColor[i*6]   = x + (r*.75) * body_vertices[i].getXComp();
				coordsAndColor[i*6+1] = y + (r*.75) * body_vertices[i].getYComp();
				coordsAndColor[i*6+2] = color.getRf();
				coordsAndColor[i*6+3] = color.getGf();
				coordsAndColor[i*6+4] = color.getBf();
				coordsAndColor[i*6+5] = color.getAf();
			}

			Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides+1)*(2+4), body_indices, Circle::numOfSides*3);
		}
	}
}

inline void Tank::drawDead(float alpha) const {
	drawBody(alpha);
	drawOutline(alpha);
}

inline void Tank::drawOutline(float alpha) const {
	if (this->dead) {
		return;
	}

	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 0.5f;

	float coordsAndColor[(Circle::numOfSides*2)*(2+4)];
	for (int i = 0; i < Circle::numOfSides; i++) {
		coordsAndColor[(i*2)  *6]   = x + (r-lineWidth) * body_vertices[i+1].getXComp();
		coordsAndColor[(i*2)  *6+1] = y + (r-lineWidth) * body_vertices[i+1].getYComp();
		coordsAndColor[(i*2+1)*6]   = x + (r+lineWidth) * body_vertices[i+1].getXComp();
		coordsAndColor[(i*2+1)*6+1] = y + (r+lineWidth) * body_vertices[i+1].getYComp();

		coordsAndColor[(i*2)  *6+2] = color.getRf();
		coordsAndColor[(i*2)  *6+3] = color.getGf();
		coordsAndColor[(i*2)  *6+4] = color.getBf();
		coordsAndColor[(i*2)  *6+5] = color.getAf();
		coordsAndColor[(i*2+1)*6+2] = color.getRf();
		coordsAndColor[(i*2+1)*6+3] = color.getGf();
		coordsAndColor[(i*2+1)*6+4] = color.getBf();
		coordsAndColor[(i*2+1)*6+5] = color.getAf();
	}

	Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides*2)*(2+4), outline_indices, Circle::numOfSides*6);
}

inline void Tank::drawShootingCooldown(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	double shootingOutlinePercent;
	if (maxShootCount*getShootingSpeedMultiplier() <= 0 || maxShootCount <= 0) {
		shootingOutlinePercent = 0;
	} else {
		shootingOutlinePercent = std::clamp<double>(shootCount/(maxShootCount*getShootingSpeedMultiplier()), 0, 1);
	}
	unsigned int shootingOutlineTriangles = Circle::numOfSides * shootingOutlinePercent;

	if (shootingOutlineTriangles > 0) {
		ColorValueHolder color = ColorValueHolder(1.0f, 1.0f, 1.0f);
		color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

		float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
		coordsAndColor[0] = x;
		coordsAndColor[1] = y;
		coordsAndColor[2] = color.getRf();
		coordsAndColor[3] = color.getGf();
		coordsAndColor[4] = color.getBf();
		coordsAndColor[5] = color.getAf();
		for (unsigned int i = 0; i <= shootingOutlineTriangles && i < Circle::numOfSides; i++) {
			SimpleVector2D vertex = SimpleVector2D(body_vertices[i % Circle::numOfSides + 1]);
			vertex.scaleAndRotate(r*(5.0/4.0), velocity.getAngle());

			coordsAndColor[(i+1)*6]   = x + vertex.getXComp();
			coordsAndColor[(i+1)*6+1] = y + vertex.getYComp();
			coordsAndColor[(i+1)*6+2] = color.getRf();
			coordsAndColor[(i+1)*6+3] = color.getGf();
			coordsAndColor[(i+1)*6+4] = color.getBf();
			coordsAndColor[(i+1)*6+5] = color.getAf();
		}

		Renderer::SubmitBatchedDraw(coordsAndColor, (shootingOutlineTriangles < Circle::numOfSides ? (shootingOutlineTriangles+2)*(2+4) : (shootingOutlineTriangles+1)*(2+4)), body_indices, shootingOutlineTriangles*3);
	}
}

inline void Tank::drawPowerCooldown(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	//first, sort by timeLeft/maxTime
	std::vector<TankPower*> sortedTankPowers(tankPowers);
	std::sort(sortedTankPowers.begin(), sortedTankPowers.end(),
		[](const TankPower* lhs, const TankPower* rhs) { return (lhs->timeLeft/lhs->maxTime > rhs->timeLeft/rhs->maxTime); });

	//second, actually draw them
	for (int i = 0; i < sortedTankPowers.size(); i++) {
		double powerOutlinePercent;
		if (sortedTankPowers[i]->maxTime <= 0) { [[unlikely]]
			powerOutlinePercent = 0;
		} else {
			powerOutlinePercent = std::clamp<double>(sortedTankPowers[i]->timeLeft/sortedTankPowers[i]->maxTime, 0, 1);
		}
		unsigned int powerOutlineTriangles = Circle::numOfSides * powerOutlinePercent;

		if (powerOutlineTriangles > 0) {
			ColorValueHolder color = sortedTankPowers[i]->getColor();
			color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

			float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
			coordsAndColor[0] = x;
			coordsAndColor[1] = y;
			coordsAndColor[2] = color.getRf();
			coordsAndColor[3] = color.getGf();
			coordsAndColor[4] = color.getBf();
			coordsAndColor[5] = color.getAf();
			for (unsigned int j = 0; j <= powerOutlineTriangles && j < Circle::numOfSides; j++) {
				SimpleVector2D vertex = SimpleVector2D(body_vertices[j % Circle::numOfSides + 1]);
				vertex.scaleAndRotate(r*(9.0/8.0), velocity.getAngle());

				coordsAndColor[(j+1)*6]   = x + vertex.getXComp();
				coordsAndColor[(j+1)*6+1] = y + vertex.getYComp();
				coordsAndColor[(j+1)*6+2] = color.getRf();
				coordsAndColor[(j+1)*6+3] = color.getGf();
				coordsAndColor[(j+1)*6+4] = color.getBf();
				coordsAndColor[(j+1)*6+5] = color.getAf();
			}

			Renderer::SubmitBatchedDraw(coordsAndColor, (powerOutlineTriangles < Circle::numOfSides ? (powerOutlineTriangles+2)*(2+4) : (powerOutlineTriangles+1)*(2+4)), body_indices, powerOutlineTriangles*3);
		}
	}
}

inline void Tank::drawMainBarrel(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 0.75f;

	float coordsAndColor[4*(2+4)];
	unsigned int indices[6];

	SimpleVector2D dist = SimpleVector2D(velocity.getAngle(), r, true);
	SimpleVector2D distCW = SimpleVector2D(velocity.getAngle() - PI/2, lineWidth, true);

	coordsAndColor[0*6]   = x                   + distCW.getXComp();
	coordsAndColor[0*6+1] = y                   + distCW.getYComp();
	coordsAndColor[1*6]   = x + dist.getXComp() + distCW.getXComp();
	coordsAndColor[1*6+1] = y + dist.getYComp() + distCW.getYComp();
	coordsAndColor[2*6]   = x + dist.getXComp() - distCW.getXComp();
	coordsAndColor[2*6+1] = y + dist.getYComp() - distCW.getYComp();
	coordsAndColor[3*6]   = x                   - distCW.getXComp();
	coordsAndColor[3*6+1] = y                   - distCW.getYComp();

	for (int i = 0; i < 4; i++) {
		coordsAndColor[i*6+2] = color.getRf();
		coordsAndColor[i*6+3] = color.getGf();
		coordsAndColor[i*6+4] = color.getBf();
		coordsAndColor[i*6+5] = color.getAf();
	}

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	Renderer::SubmitBatchedDraw(coordsAndColor, 4*(2+4), indices, 6);
}

inline void Tank::drawExtraBarrels(float alpha) const {
	if (shootingPoints.size() <= 1) {
		return;
	}

	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = ColorValueHolder(0.75f, 0.75f, 0.75f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 0.375f; //TODO: unsure what to put this at; .25 too thin, .5 too thick, .375 looks a bit off from aliasing

	float* coordsAndColor = new float[(shootingPoints.size()-1)*4*(2+4)];
	unsigned int* indices = new unsigned int[(shootingPoints.size()-1)*6];

	for (int i = 0; i < shootingPoints.size()-1; i++) {
		const int startVertex = (i*4) * 6;
		const int startIndex = i*6;

		SimpleVector2D dist = SimpleVector2D(getEvaluatedCannonAngle(i+1), r, true);
		SimpleVector2D distCW = SimpleVector2D(getEvaluatedCannonAngle(i+1) - PI/2, lineWidth, true);

		coordsAndColor[startVertex + 0*6]   = x                   + distCW.getXComp();
		coordsAndColor[startVertex + 0*6+1] = y                   + distCW.getYComp();
		coordsAndColor[startVertex + 1*6]   = x + dist.getXComp() + distCW.getXComp();
		coordsAndColor[startVertex + 1*6+1] = y + dist.getYComp() + distCW.getYComp();
		coordsAndColor[startVertex + 2*6]   = x + dist.getXComp() - distCW.getXComp();
		coordsAndColor[startVertex + 2*6+1] = y + dist.getYComp() - distCW.getYComp();
		coordsAndColor[startVertex + 3*6]   = x                   - distCW.getXComp();
		coordsAndColor[startVertex + 3*6+1] = y                   - distCW.getYComp();

		for (int j = 0; j < 4; j++) {
			coordsAndColor[startVertex + j*6+2] = color.getRf();
			coordsAndColor[startVertex + j*6+3] = color.getGf();
			coordsAndColor[startVertex + j*6+4] = color.getBf();
			coordsAndColor[startVertex + j*6+5] = color.getAf();
		}

		indices[startIndex + 0] = startVertex/6 + 0;
		indices[startIndex + 1] = startVertex/6 + 1;
		indices[startIndex + 2] = startVertex/6 + 2;
		indices[startIndex + 3] = startVertex/6 + 2;
		indices[startIndex + 4] = startVertex/6 + 3;
		indices[startIndex + 5] = startVertex/6 + 0;
	}

	Renderer::SubmitBatchedDraw(coordsAndColor, (shootingPoints.size()-1)*4*(2+4), indices, (shootingPoints.size()-1)*6);
	delete[] coordsAndColor; delete[] indices;
}

inline void Tank::drawExtraExtraBarrels(float alpha) const {
	if (extraShootingPoints.size() <= 1) {
		return;
	}

	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = ColorValueHolder(0.25f, 0.25f, 0.25f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 0.375f;

	float* coordsAndColor = new float[(extraShootingPoints.size()-1)*4*(2+4)];
	unsigned int* indices = new unsigned int[(extraShootingPoints.size()-1)*6];

	for (int i = 0; i < extraShootingPoints.size()-1; i++) {
		const int startVertex = (i*4) * 6;
		const int startIndex = i*6;

		SimpleVector2D distFromCenter = SimpleVector2D(getEvaluatedCannonAngle(0, i+1), r, true);
		const float extraCannonLength = r * (pow(sin(shootingPoints[0].angleFromCenter + extraShootingPoints[i+1].angleFromCenter),2) * .25 + .25); //.25 at main cannon, .5 at 90deg //pow instead of abs because 45deg cannon was too long
		SimpleVector2D dist = SimpleVector2D(getEvaluatedCannonAngleWithEdge(0, i+1), extraCannonLength, true);
		SimpleVector2D distCW = SimpleVector2D(getEvaluatedCannonAngleWithEdge(0, i+1) - PI/2, lineWidth, true);

		coordsAndColor[startVertex + 0*6]   = x + distFromCenter.getXComp()                   + distCW.getXComp();
		coordsAndColor[startVertex + 0*6+1] = y + distFromCenter.getYComp()                   + distCW.getYComp();
		coordsAndColor[startVertex + 1*6]   = x + distFromCenter.getXComp() + dist.getXComp() + distCW.getXComp();
		coordsAndColor[startVertex + 1*6+1] = y + distFromCenter.getYComp() + dist.getYComp() + distCW.getYComp();
		coordsAndColor[startVertex + 2*6]   = x + distFromCenter.getXComp() + dist.getXComp() - distCW.getXComp();
		coordsAndColor[startVertex + 2*6+1] = y + distFromCenter.getYComp() + dist.getYComp() - distCW.getYComp();
		coordsAndColor[startVertex + 3*6]   = x + distFromCenter.getXComp()                   - distCW.getXComp();
		coordsAndColor[startVertex + 3*6+1] = y + distFromCenter.getYComp()                   - distCW.getYComp();

		for (int j = 0; j < 4; j++) {
			coordsAndColor[startVertex + j*6+2] = color.getRf();
			coordsAndColor[startVertex + j*6+3] = color.getGf();
			coordsAndColor[startVertex + j*6+4] = color.getBf();
			coordsAndColor[startVertex + j*6+5] = color.getAf();
		}

		indices[startIndex + 0] = startVertex/6 + 0;
		indices[startIndex + 1] = startVertex/6 + 1;
		indices[startIndex + 2] = startVertex/6 + 2;
		indices[startIndex + 3] = startVertex/6 + 2;
		indices[startIndex + 4] = startVertex/6 + 3;
		indices[startIndex + 5] = startVertex/6 + 0;
	}

	Renderer::SubmitBatchedDraw(coordsAndColor, (extraShootingPoints.size()-1)*4*(2+4), indices, (extraShootingPoints.size()-1)*6);
	delete[] coordsAndColor; delete[] indices;
}

bool Tank::kill() {
	//for checking whether to save the tank, it's first come first served, then no one else
	//if it can save the tank, stop checking, else continue checking

	bool shouldBeKilled = true;

	for (int i = 0; i < tankPowers.size(); i++) {
		bool killTankPower = false;

		if (tankPowers[i]->modifiesDeathHandling) {
			InteractionBoolHolder check_temp = tankPowers[i]->modifiedDeathHandling(this);
			if (!check_temp.shouldDie) {
				shouldBeKilled = false;
			}
			if (check_temp.otherShouldDie) {
				killTankPower = true;
			}
		}

		if (killTankPower) {
			removePower(i);
			i--;
			//continue;
		}
		if (!shouldBeKilled) {
			break;
		}
	}

	if (shouldBeKilled) {
		this->dead = shouldBeKilled;
	}

	return this->dead;
}

void Tank::kill_hard() {
	this->dead = true;
}

float Tank::getHighestOffenseImportance() const {
	float highest = LOW_IMPORTANCE;
	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->getOffenseImportance() > highest) {
			highest = tankPowers[i]->getOffenseImportance();
		}
	}
	return highest;
}

float Tank::getHighestOffenseTier(float importance) const {
	if (tankPowers.size() == 0) {
		return 0;
	}
	float highest = LOW_TIER;
	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->getOffenseImportance() == importance) {
			if (tankPowers[i]->getOffenseTier(this) > highest) {
				highest = tankPowers[i]->getOffenseTier(this);
			}
		}
	}
	return highest;
}

float Tank::getOffenseTier() const {
	return getHighestOffenseTier(getHighestOffenseImportance());
}

float Tank::getHighestDefenseImportance() const {
	float highest = LOW_IMPORTANCE;
	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->getDefenseImportance() > highest) {
			highest = tankPowers[i]->getDefenseImportance();
		}
	}
	return highest;
}

float Tank::getHighestDefenseTier(float importance) const {
	if (tankPowers.size() == 0) {
		return 0;
	}
	float highest = LOW_TIER;
	for (int i = 0; i < tankPowers.size(); i++) {
		if (tankPowers[i]->getDefenseImportance() == importance) {
			if (tankPowers[i]->getDefenseTier(this) > highest) {
				highest = tankPowers[i]->getDefenseTier(this);
			}
		}
	}
	return highest;
}

float Tank::getDefenseTier() const {
	return getHighestDefenseTier(getHighestDefenseImportance());
}
