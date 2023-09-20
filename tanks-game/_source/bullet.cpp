#include "bullet.h"

#include "constants.h"
#include <cmath>
#include <algorithm> //std::clamp, std::min
#include <iostream>

#include "renderer.h"
#include "color-mixer.h"
#include "background-rect.h"

const double Bullet::default_radius = 4;
Bullet::Bullet(double x_, double y_, double angle, Team_ID teamID, BulletParentType parentType, Game_ID parentID) : GameThing(teamID) { //every bullet constructor does this stuff
	this->x = x_;
	this->y = y_;
	this->velocity = SimpleVector2D(angle, 0, true);
	this->parentType = parentType;
	this->parentID = parentID;
	this->lifeValue = 100;
}

Bullet::Bullet(double x_, double y_, double angle, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp) : Bullet(x_,y_,angle,teamID,parentType,parentID) {
	bulletPowers.reserve(bp->size());
	for (int i = 0; i < bp->size(); i++) {
		bulletPowers.push_back(bp->at(i));
	}
	for (int i = 0; i < bulletPowers.size(); i++) {
		bulletPowers[i]->initialize(this);
	}
	//bp not deleted
}

//probably just for banana bullet creation:
Bullet::Bullet(double x_, double y_, double r_, double angle, double vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp, bool) : Bullet(x_,y_,angle,teamID,parentType,parentID,bp) {
	this->r = r_;
	this->velocity.setMagnitude(vel); // * getBulletSpeedMultiplier(); //not wanted
	this->initial_velocity = velocity.getMagnitude();
	this->acceleration = getBulletAcceleration();
}

//avoid using:
Bullet::Bullet(double x_, double y_, double r_, double angle, double vel, double acc, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp, bool) : Bullet(x_,y_,angle,teamID,parentType,parentID,bp) {
	this->r = r_ * getBulletRadiusMultiplier();
	this->velocity.setMagnitude(vel * getBulletSpeedMultiplier());
	this->initial_velocity = this->velocity.getMagnitude();
	this->acceleration = acc;
}

//regular 1:
Bullet::Bullet(double x_, double y_, double r_, double angle, double vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID) : Bullet(x_,y_,angle,teamID,parentType,parentID) {
	this->r = r_;
	this->velocity.setMagnitude(vel);
	this->initial_velocity = this->velocity.getMagnitude();
	this->acceleration = 0;
}

//regular 2:
Bullet::Bullet(double x_, double y_, double r_, double angle, double vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID, std::vector<BulletPower*>* bp) : Bullet(x_,y_,angle,teamID,parentType,parentID,bp) {
	this->r = r_ * getBulletRadiusMultiplier();
	this->velocity.setMagnitude(vel * getBulletSpeedMultiplier());
	this->initial_velocity = this->velocity.getMagnitude();
	this->acceleration = getBulletAcceleration();
}

Bullet::~Bullet() {
	for (int i = 0; i < bulletPowers.size(); i++) {
		delete bulletPowers[i];
	}
	bulletPowers.clear();
}

bool Bullet::canCollideWith(const GameThing* thing) const {
	if (this->parentType == BulletParentType::team) {
		return (this->getTeamID() != thing->getTeamID());
	}
	if (this->parentType == BulletParentType::individual) {
		return (parentID != thing->getGameID());
	}
	/*
	if (this->parentType == BulletParentType::name) {
		//TODO
	}
	*/
	return true;
}

bool Bullet::canCollideWith(const Bullet* b_other) const {
	if (this->parentType == BulletParentType::individual) {
		return ((parentID != b_other->getGameID()) && (parentID != b_other->getParentID()));
	}
	return canCollideWith((const GameThing*)b_other);
}

void Bullet::update(const BulletUpdateStruct* up) {
	this->x += up->x;
	this->y += up->y;
	this->r += up->r;
	this->velocity = SimpleVector2D(velocity.getAngle() + up->angle, velocity.getMagnitude() + up->speed, true);
	this->lifeValue = std::min(this->lifeValue + up->alpha, 100.0);
}

double Bullet::getAngle() const {
	return fmod(fmod(velocity.getAngle(), 2*PI) + 2*PI, 2*PI);
}

bool Bullet::move() {
	bool shouldBeKilled = false;
	bool modifiedMovement = false;
	bool overridedMovement = false;
	bool noMoreMovementSpecials = false;
	//TODO: handle killing the bulletpowers

	for (int k = 0; k < bulletPowers.size(); k++) {
		if (bulletPowers[k]->modifiesMovement) {
			if (bulletPowers[k]->modifiedMovementCanOnlyWorkIndividually && modifiedMovement) {
				continue;
			}
			if (noMoreMovementSpecials) {
				continue;
			}

			modifiedMovement = true;
			if (bulletPowers[k]->overridesMovement) {
				overridedMovement = true;
			}
			if (!bulletPowers[k]->modifiedMovementCanWorkWithOthers) {
				noMoreMovementSpecials = true;
			}

			InteractionBoolHolder check_temp = bulletPowers[k]->modifiedMovement(this);
			if (check_temp.shouldDie) {
				shouldBeKilled = true;
				overridedMovement = true;
				break;
			}
		}
	}

	if (!overridedMovement) {
		move_base();
	}

	return shouldBeKilled;
}

inline void Bullet::move_base() {
	velocity.changeMagnitude(acceleration);
	x += velocity.getXComp();
	y += velocity.getYComp();
}

double Bullet::getBulletSpeedMultiplier() const {
	//look at Tank::getTankFiringRateMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	for (int i = 0; i < bulletPowers.size(); i++) {
		double value = bulletPowers[i]->getBulletSpeedMultiplier();
		if (bulletPowers[i]->bulletSpeedStacks) {
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

	return highest * lowest * value;
}

double Bullet::getBulletRadiusMultiplier() const {
	//look at Tank::getTankFiringRateMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	for (int i = 0; i < bulletPowers.size(); i++) {
		double value = bulletPowers[i]->getBulletRadiusMultiplier();
		if (bulletPowers[i]->bulletRadiusStacks) {
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
	return highest * lowest * value;
}

double Bullet::getBulletAcceleration() const {
	//look at Tank::getTankFiringRateMultiplier()
	//(this has importance though)

	if (bulletPowers.size() == 0) {
		return 0;
	}

	float importance = LOW_IMPORTANCE;
	for (int i = 0; i < bulletPowers.size(); i++) {
		float value = bulletPowers[i]->getBulletAccelerationImportance();
		if (value > importance) {
			importance = value;
		}
	}

	double highest = 0;
	double lowest = 0;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getBulletAccelerationImportance() == importance) {
			double value = bulletPowers[i]->getBulletAcceleration();
			if (value < 0 && value < lowest) {
				lowest = value;
			} else if (value > 0 && value > highest) {
				highest = value;
			}
		}
	}
	return highest + lowest;
	//return (abs(highest) > abs(lowest) ? highest : lowest);
}

void Bullet::powerCalculate() {
	for (int i = bulletPowers.size() - 1; i >= 0; i--) {
		bulletPowers[i]->tick(this); //I don't think any power will use this, but whatever
		if (bulletPowers[i]->isDone()) { [[unlikely]]
			removePower(i);
		} else { //to make each power last its full length, not n-1 length
			bulletPowers[i]->powerTick();
		}
	}
}

void Bullet::removePower(int i) {
	bulletPowers[i]->removeEffects(this);
	delete bulletPowers[i];
	bulletPowers.erase(bulletPowers.begin() + i);
}

ColorValueHolder Bullet::getColor() const {
	if (bulletPowers.size() == 0) {
		return defaultColor;
	} else {
		float highest = LOW_IMPORTANCE;
		for (int i = 0; i < bulletPowers.size(); i++) {
			if (bulletPowers[i]->getColorImportance() > highest) {
				highest = bulletPowers[i]->getColorImportance();
			}
		}
		if (highest < 0) {
			return defaultColor;
		}
		std::vector<ColorValueHolder> mixingColors;
		for (int i = 0; i < bulletPowers.size(); i++) {
			if (bulletPowers[i]->getColorImportance() == highest) {
				mixingColors.push_back(bulletPowers[i]->getColor());
			}
		}
		return ColorMixer::mix(mixingColors.data(), mixingColors.size());
	}
}

void Bullet::draw() const {
	drawDeathCooldown();
	drawBody();
	drawOutline();
}

void Bullet::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Bullet::draw!" << std::endl;
		case DrawingLayers::normal:
			draw();
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void Bullet::poseDraw() const {
	drawBody();
	drawOutline();
}

void Bullet::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Bullet::poseDraw!" << std::endl;
		case DrawingLayers::normal:
			poseDraw();
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void Bullet::ghostDraw(float alpha) const {
	drawDeathCooldown(alpha);
	drawBody(alpha);
	drawOutline(alpha);
}

void Bullet::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Bullet::ghostDraw!" << std::endl;
		case DrawingLayers::normal:
			ghostDraw(alpha);
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

inline void Bullet::drawBody(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = getColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

	float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
	coordsAndColor[0] = x;
	coordsAndColor[1] = y;
	coordsAndColor[2] = color.getRf();
	coordsAndColor[3] = color.getGf();
	coordsAndColor[4] = color.getBf();
	coordsAndColor[5] = color.getAf();
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		coordsAndColor[i*6]   = x + r * cos((i-1) * (2*PI / Circle::numOfSides));
		coordsAndColor[i*6+1] = y + r * sin((i-1) * (2*PI / Circle::numOfSides));
		coordsAndColor[i*6+2] = color.getRf();
		coordsAndColor[i*6+3] = color.getGf();
		coordsAndColor[i*6+4] = color.getBf();
		coordsAndColor[i*6+5] = color.getAf();
	}

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = 0;
		indices[i*3+1] = i+1;
		indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides+1)*(2+4), indices, Circle::numOfSides*3);
}

inline void Bullet::drawOutline(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 0.5f;

	float coordsAndColor[(Circle::numOfSides*2)*(2+4)];
	for (int i = 0; i < Circle::numOfSides; i++) {
		coordsAndColor[(i*2)  *6]   = x + (r-lineWidth) * cos(i * (2*PI / Circle::numOfSides));
		coordsAndColor[(i*2)  *6+1] = y + (r-lineWidth) * sin(i * (2*PI / Circle::numOfSides));
		coordsAndColor[(i*2+1)*6]   = x + (r+lineWidth) * cos(i * (2*PI / Circle::numOfSides));
		coordsAndColor[(i*2+1)*6+1] = y + (r+lineWidth) * sin(i * (2*PI / Circle::numOfSides));

		coordsAndColor[(i*2)  *6+2] = color.getRf();
		coordsAndColor[(i*2)  *6+3] = color.getGf();
		coordsAndColor[(i*2)  *6+4] = color.getBf();
		coordsAndColor[(i*2)  *6+5] = color.getAf();
		coordsAndColor[(i*2+1)*6+2] = color.getRf();
		coordsAndColor[(i*2+1)*6+3] = color.getGf();
		coordsAndColor[(i*2+1)*6+4] = color.getBf();
		coordsAndColor[(i*2+1)*6+5] = color.getAf();
	}

	unsigned int indices[Circle::numOfSides*6];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*6]   =  i*2;
		indices[i*6+1] =  i*2+1;
		indices[i*6+2] = (i*2+3) % (Circle::numOfSides*2);
		indices[i*6+3] = (i*2+3) % (Circle::numOfSides*2);
		indices[i*6+4] = (i*2+2) % (Circle::numOfSides*2);
		indices[i*6+5] =  i*2;
	}

	Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides*2)*(2+4), indices, Circle::numOfSides*6);
}

inline void Bullet::drawDeathCooldown(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	//checking glIsEnabled(GL_BLEND) to skip is an option (though the result should be stored somewhere to avoid GL calls)

	if (this->lifeValue < 100) {
		double deathPercent = std::clamp<double>(this->lifeValue/100, 0, 1);
		unsigned int deathTriangles = Circle::numOfSides * deathPercent;

		if (deathTriangles > 0) {
			ColorValueHolder color = ColorValueHolder(1.0f, 1.0f, 1.0f);
			color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

			float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
			coordsAndColor[0] = x;
			coordsAndColor[1] = y;
			coordsAndColor[2] = color.getRf();
			coordsAndColor[3] = color.getGf();
			coordsAndColor[4] = color.getBf();
			coordsAndColor[5] = color.getAf();
			for (unsigned int i = 0; i <= deathTriangles && i < Circle::numOfSides; i++) {
				//the final triangle shares its last vertex with the first triangle, which is why this loop condition is a bit strange (second conditional only false for that last triangle)
				//with wrong condition: two verts on an old bullet's death outline to the center of a new bullet's center body or death outline, though sometimes even a tank or rarely the bottom left corner (why)
				//to be more specific: with the old conditional, think it was happening when deathTriangles==1, leading to pushing only two total verts but pushing three indices; but that would mean it was always pushing insufficient verts for the indices, why wasn't it showing up before?
				coordsAndColor[(i+1)*6]   = x + ((r+2)*(9.0/8.0)) * cos(PI/2 + i * (2*PI / Circle::numOfSides));
				coordsAndColor[(i+1)*6+1] = y + ((r+2)*(9.0/8.0)) * sin(PI/2 + i * (2*PI / Circle::numOfSides));
				coordsAndColor[(i+1)*6+2] = color.getRf();
				coordsAndColor[(i+1)*6+3] = color.getGf();
				coordsAndColor[(i+1)*6+4] = color.getBf();
				coordsAndColor[(i+1)*6+5] = color.getAf();
			}

			unsigned int indices[Circle::numOfSides*3];
			for (unsigned int i = 0; i < deathTriangles; i++) {
				indices[i*3]   = 0;
				indices[i*3+1] = i+1;
				indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
			}

			Renderer::SubmitBatchedDraw(coordsAndColor, (deathTriangles < Circle::numOfSides ? (deathTriangles+2)*(2+4) : (deathTriangles+1)*(2+4)), indices, deathTriangles*3);
		}
	}
}

bool Bullet::kill() {
	//see Tank::kill()

	bool shouldBeKilled = true;

	for (int i = 0; i < bulletPowers.size(); i++) {
		bool killBulletPower = false;

		if (bulletPowers[i]->modifiesDeathHandling) {
			InteractionBoolHolder check_temp = bulletPowers[i]->modifiedDeathHandling(this);
			if (!check_temp.shouldDie) {
				shouldBeKilled = false;
			}
			if (check_temp.otherShouldDie) {
				killBulletPower = true;
			}
		}

		if (killBulletPower) {
			removePower(i);
			i--;
			//continue;
		}
		if (!shouldBeKilled) {
			break;
		}
	}

	return shouldBeKilled;
}

float Bullet::getHighestOffenseImportance() const {
	float highest = LOW_IMPORTANCE;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getOffenseImportance() > highest) {
			highest = bulletPowers[i]->getOffenseImportance();
		}
	}
	return highest;
}

float Bullet::getHighestOffenseTier(float importance) const {
	if (bulletPowers.size() == 0) {
		return 0;
	}
	float highest = LOW_TIER;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getOffenseImportance() == importance) {
			if (bulletPowers[i]->getOffenseTier(this) > highest) {
				highest = bulletPowers[i]->getOffenseTier(this);
			}
		}
	}
	return highest;
}

float Bullet::getOffenseTier() const {
	return getHighestOffenseTier(getHighestOffenseImportance());
}

float Bullet::getHighestDefenseImportance() const {
	float highest = LOW_IMPORTANCE;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getDefenseImportance() > highest) {
			highest = bulletPowers[i]->getDefenseImportance();
		}
	}
	return highest;
}

float Bullet::getHighestDefenseTier(float importance) const {
	if (bulletPowers.size() == 0) {
		return 0;
	}
	float highest = LOW_TIER;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getDefenseImportance() == importance) {
			if (bulletPowers[i]->getDefenseTier(this) > highest) {
				highest = bulletPowers[i]->getDefenseTier(this);
			}
		}
	}
	return highest;
}

float Bullet::getDefenseTier() const {
	return getHighestDefenseTier(getHighestDefenseImportance());
}

BulletUpdateStruct::BulletUpdateStruct(double x, double y, double r, double speed, double angle, double alpha, const std::vector<Bullet*>& newBullets) {
	//add acceleration?
	this->x = x;
	this->y = y;
	this->r = r;
	this->speed = speed;
	this->angle = angle;
	this->alpha = alpha;
	this->newBullets = std::vector<Bullet*>(newBullets);
}

BulletUpdateStruct::BulletUpdateStruct(double x, double y, double r, double speed, double angle, double alpha) {
	//add acceleration?
	this->x = x;
	this->y = y;
	this->r = r;
	this->speed = speed;
	this->angle = angle;
	this->alpha = alpha;
	this->newBullets = std::vector<Bullet*>();
}

void BulletUpdateStruct::add(const BulletUpdateStruct& other) {
	this->x += other.x;
	this->y += other.y;
	this->r += other.r;
	this->speed += other.speed;
	this->angle += other.angle;
	this->alpha += other.alpha;
	newBullets.insert(newBullets.end(), other.newBullets.begin(), other.newBullets.end());
}
