#include "bullet.h"

#include "constants.h"
#include <cmath>
#include <algorithm> //std::clamp, std::min, std::max
#include <iostream>

#include "renderer.h"
#include "color-cache.h"
#include "color-mixer.h"
#include "background-rect.h"

#include "game-manager.h" //settings file

SimpleVector2D Bullet::body_vertices[Bullet::BulletSideCount + 1];
unsigned int Bullet::body_indices[Bullet::BulletSideCount * 3];
unsigned int Bullet::outline_indices[Bullet::BulletSideCount * 2*3];
bool Bullet::initialized_vertices = false;

const ColorValueHolder Bullet::default_color = ColorValueHolder(0.5f, 0.5f, 0.5f);
const double Bullet::default_radius = 4;

//TODO: bitset for powers' bools? possibly more cache-friendly but initialization might take longer

Bullet::Bullet(double x_, double y_, float angle, Team_ID teamID, BulletParentType parentType, Game_ID parentID) : GameThing(teamID, ObjectType::Bullet) { //every bullet constructor does this stuff
	this->x = x_;
	this->y = y_;
	this->velocity = SimpleVector2D(angle, 0, true);
	this->parentType = parentType;
	this->parentID = parentID;
	this->lifeValue = 100;
	this->m_colorIdentifier = 0;

	//initializeVertices(); //called in main() to reduce the tiny overhead in calling this every time a bullet is created
}

Bullet::Bullet(double x_, double y_, float angle, Team_ID teamID, BulletParentType parentType, Game_ID parentID, const std::vector<BulletPower*>* bp) : Bullet(x_,y_,angle,teamID,parentType,parentID) {
	bulletPowers.reserve(bp->size());
	for (int i = 0; i < bp->size(); i++) {
		bulletPowers.push_back(bp->data()[i]);
	}
	for (int i = 0; i < bulletPowers.size(); i++) {
		bulletPowers[i]->initialize(this);
	}
	updateColorIdentifier(); //TODO: regenerating this each time is not optimal, but oh well
	//bp not deleted
}

//probably just for banana bullet creation:
Bullet::Bullet(double x_, double y_, double r_, float angle, float vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID, const std::vector<BulletPower*>* bp, bool) : Bullet(x_,y_,angle,teamID,parentType,parentID,bp) {
	this->r = r_;
	this->velocity.setMagnitude(vel); // * getBulletSpeedMultiplier(); //not wanted
	this->initial_velocity = this->velocity.getMagnitude();
	this->acceleration = getBulletAcceleration();
}

//avoid using:
Bullet::Bullet(double x_, double y_, double r_, float angle, float vel, float acc, Team_ID teamID, BulletParentType parentType, Game_ID parentID, const std::vector<BulletPower*>* bp, bool) : Bullet(x_,y_,angle,teamID,parentType,parentID,bp) {
	this->r = r_ * getBulletRadiusMultiplier();
	this->velocity.setMagnitude(vel * getBulletSpeedMultiplier());
	this->initial_velocity = this->velocity.getMagnitude();
	this->acceleration = acc;
}

//regular 1:
Bullet::Bullet(double x_, double y_, double r_, float angle, float vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID) : Bullet(x_,y_,angle,teamID,parentType,parentID) {
	this->r = r_;
	this->velocity.setMagnitude(vel);
	this->initial_velocity = this->velocity.getMagnitude();
	this->acceleration = 0;
}

//regular 2:
Bullet::Bullet(double x_, double y_, double r_, float angle, float vel, Team_ID teamID, BulletParentType parentType, Game_ID parentID, const std::vector<BulletPower*>* bp) : Bullet(x_,y_,angle,teamID,parentType,parentID,bp) {
	this->r = r_ * getBulletRadiusMultiplier();
	this->velocity.setMagnitude(vel * getBulletSpeedMultiplier());
	this->initial_velocity = this->velocity.getMagnitude();
	this->acceleration = getBulletAcceleration();
}

Bullet::~Bullet() {
	for (int i = 0; i < bulletPowers.size(); i++) {
		delete bulletPowers[i];
	}
	//bulletPowers.clear();
}

bool Bullet::initializeVertices() {
	if (initialized_vertices) [[likely]] {
		return false;
	}

	body_vertices[0] = SimpleVector2D(0, 0);
	for (int i = 1; i < Bullet::BulletSideCount+1; i++) {
		body_vertices[i] = SimpleVector2D(std::cos((i-1) * (2*PI / Bullet::BulletSideCount) + PI/2), std::sin((i-1) * (2*PI / Bullet::BulletSideCount) + PI/2));
	}

	for (int i = 0; i < Bullet::BulletSideCount; i++) {
		body_indices[i*3]   = 0;
		body_indices[i*3+1] = i+1;
		body_indices[i*3+2] = (i+1) % Bullet::BulletSideCount + 1;
	}

	for (int i = 0; i < Bullet::BulletSideCount; i++) {
		outline_indices[i*6]   =  i*2;
		outline_indices[i*6+1] =  i*2+1;
		outline_indices[i*6+2] = (i*2+3) % (Bullet::BulletSideCount*2);
		outline_indices[i*6+3] = (i*2+3) % (Bullet::BulletSideCount*2);
		outline_indices[i*6+4] = (i*2+2) % (Bullet::BulletSideCount*2);
		outline_indices[i*6+5] =  i*2;
	}

	initialized_vertices = true;
	return true;
}

bool Bullet::canCollideWith(const GameThing* thing) const {
	switch (this->parentType) {
		case BulletParentType::team:
			return (this->getTeamID() != thing->getTeamID());
		case BulletParentType::individual:
			return (parentID != thing->getGameID());
		case BulletParentType::name:
			//TODO
			[[fallthrough]];
		case BulletParentType::none:
			//TODO
			[[fallthrough]];
		[[unlikely]] default:
			return true;
	}
}

bool Bullet::canCollideWith(const Bullet* b_other) const {
	if (this->parentType == BulletParentType::individual) {
		return ((parentID != b_other->getGameID()) && (parentID != b_other->getParentID()));
	}
	return canCollideWith(static_cast<const GameThing*>(b_other));
}

void Bullet::update(const BulletUpdateStruct* up) {
	this->x += up->x;
	this->y += up->y;
	this->r += up->r;
	this->velocity = SimpleVector2D(velocity.getAngle() + up->angle, velocity.getMagnitude() + up->speed, true);
	this->lifeValue = std::min(this->lifeValue + up->alpha, 100.0f);
}

bool Bullet::move() {
	bool shouldBeKilled = false;
	bool overridedMovement = false;
	//TODO: handle killing the bulletpowers

	for (int k = 0; k < bulletPowers.size(); k++) {
		if (bulletPowers[k]->modifiesMovement) {
			if (bulletPowers[k]->overridesMovement) {
				overridedMovement = true;
			}

			InteractionBoolHolder check_temp = bulletPowers[k]->modifiedMovement(this);
			if (check_temp.shouldDie) {
				shouldBeKilled = true;
				overridedMovement = true;
				break;
			}

			if (!bulletPowers[k]->modifiedMovementCanWorkWithOthers) {
				break;
			}
		}
	}

	if (!overridedMovement) {
		move_base();
	}

	degradeHandle();
	growHandle();

	return shouldBeKilled;
}

inline void Bullet::move_base() {
	velocity.changeMagnitude(acceleration);
	x += velocity.getXComp();
	y += velocity.getYComp();
}

inline void Bullet::degradeHandle() {
	if (velocity.getMagnitude() == 0) {
		this->lifeValue -= getBulletDegradeAmount();
	}
}

inline void Bullet::growHandle() {
	double additiveR, multiplierR;
	if (velocity.getMagnitude() > 0) {
		std::pair<double, double> growValues = getBulletRadiusGrowNumber_Moving();
		additiveR = growValues.first;
		multiplierR = growValues.second;
	} else {
		std::pair<double, double> growValues = getBulletRadiusGrowNumber_Stationary();
		additiveR = growValues.first;
		multiplierR = growValues.second;
	}

	//if multiplierR>1, choose whichever is greater; else apply both (add then mult)
	if (multiplierR > 1) {
		if (this->r + additiveR >= this->r * multiplierR) {
			//this means additiveR > the radius increase multiplierR will bring
			this->r += additiveR;
		} else {
			this->r *= multiplierR;
		}
	} else {
		this->r += additiveR;
		this->r *= multiplierR;
	}

	//TODO: negative additive values could work, as long as when b->r <= 0 the bullet dies
}

float Bullet::getBulletSpeedMultiplier() const {
	//look at Tank::getFiringRateMultiplier()

	float highest = 1;
	float lowest = 1;
	std::vector<float> stackList;

	for (int i = 0; i < bulletPowers.size(); i++) {
		float value = bulletPowers[i]->getBulletSpeedMultiplier();
		if (bulletPowers[i]->bulletSpeedStacks) {
			stackList.push_back(value);
		} else {
			if (value < lowest) {
				lowest = value;
			} else if (value > highest) {
				highest = value;
			}
		}
	}

	float value = 1;
	for (int i = 0; i < stackList.size(); i++) {
		value *= stackList[i];
	}

	return highest * lowest * value;
}

double Bullet::getBulletRadiusMultiplier() const {
	//look at Tank::getFiringRateMultiplier()

	double highest = 1;
	double lowest = 1;
	std::vector<double> stackList;

	for (int i = 0; i < bulletPowers.size(); i++) {
		double value = bulletPowers[i]->getBulletRadiusMultiplier();
		if (bulletPowers[i]->bulletRadiusStacks) {
			stackList.push_back(value);
		} else {
			if (value < lowest) {
				lowest = value;
			} else if (value > highest) {
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

float Bullet::getBulletAcceleration() const {
	//look at Tank::getFiringRateMultiplier()
	//(this has importance though)

	float importance = LOW_IMPORTANCE;
	for (int i = 0; i < bulletPowers.size(); i++) {
		float value = bulletPowers[i]->getBulletAccelerationImportance();
		if (value > importance) {
			importance = value;
		}
	}

	float highest = 0;
	float lowest = 0;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getBulletAccelerationImportance() == importance) {
			float value = bulletPowers[i]->getBulletAcceleration();
			if (value < lowest) {
				lowest = value;
			} else if (value > highest) {
				highest = value;
			}
		}
	}
	return highest + lowest;
}

float Bullet::getBulletDegradeAmount() const {
	//look at Tank::getFiringRateMultiplier()
	//(this has importance though)
	//negative degrade values are not valid, so this just finds the highest value

	float importance = LOW_IMPORTANCE;
	for (int i = 0; i < bulletPowers.size(); i++) {
		float value = bulletPowers[i]->getBulletDegradeImportance();
		if (value > importance) {
			importance = value;
		}
	}

	float highest = 0;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getBulletDegradeImportance() == importance) {
			float value = bulletPowers[i]->getBulletDegradeAmount();
			if (value > highest) {
				highest = value;
			}
		}
	}
	return highest;
}

std::pair<double, double> Bullet::getBulletRadiusGrowNumber_Stationary() const {
	//look at Tank::getFiringRateMultiplier()
	//negative values are not valid, but not enforced

	double highestAdditive = 0;
	double highestMultiplier = 1;
	double lowestMultiplier = 1;

	for (int i = 0; i < bulletPowers.size(); i++) {
		double value = bulletPowers[i]->getBulletRadiusGrowNumber_Stationary();
		if (bulletPowers[i]->bulletRadiusGrowMultiplies_Stationary) {
			if (value < lowestMultiplier) {
				lowestMultiplier = value;
			} else if (value > highestMultiplier) {
				highestMultiplier = value;
			}
		} else {
			if (value > highestAdditive) {
				highestAdditive = value;
			}
		}
	}
	return { highestAdditive, highestMultiplier * lowestMultiplier };
}

std::pair<double, double> Bullet::getBulletRadiusGrowNumber_Moving() const {
	//look at Tank::getFiringRateMultiplier()
	//negative values are not valid, but not enforced

	double highestAdditive = 0;
	double highestMultiplier = 1;
	double lowestMultiplier = 1;

	for (int i = 0; i < bulletPowers.size(); i++) {
		double value = bulletPowers[i]->getBulletRadiusGrowNumber_Moving();
		if (bulletPowers[i]->bulletRadiusGrowMultiplies_Moving) {
			if (value < lowestMultiplier) {
				lowestMultiplier = value;
			} else if (value > highestMultiplier) {
				highestMultiplier = value;
			}
		} else {
			if (value > highestAdditive) {
				highestAdditive = value;
			}
		}
	}
	return { highestAdditive, highestMultiplier * lowestMultiplier };
}

void Bullet::powerTick() {
	for (int i = bulletPowers.size() - 1; i >= 0; i--) {
		bulletPowers[i]->tick(this); //I don't think any power will use this, but whatever
	}
}

void Bullet::removePower(int i) {
	bulletPowers[i]->removeEffects(this);
	delete bulletPowers[i];
	bulletPowers.erase(bulletPowers.begin() + i);
	updateColorIdentifier();
}

void Bullet::updateColorIdentifier() {
	float highest = LOW_IMPORTANCE;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getColorImportance() > highest) {
			highest = bulletPowers[i]->getColorImportance();
		}
	}
	if (highest < 0) [[unlikely]] {
		m_colorIdentifier = 0;
		return;
	}

	std::string colorIdentifierStr = "";
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getColorImportance() == highest) {
			colorIdentifierStr += "|" + bulletPowers[i]->getColorIdentifier();
		}
	}
	m_colorIdentifier = ColorCacheBullet::getColorID(colorIdentifierStr);
}

const ColorValueHolder& Bullet::getColor() const {
	if (m_colorIdentifier == 0) [[unlikely]] {
		return default_color;
	}

	if (ColorCacheBullet::colorIsCached(m_colorIdentifier)) [[likely]] {
		return ColorCacheBullet::getColor(m_colorIdentifier);
	} else {
		float highest = LOW_IMPORTANCE;
		for (int i = 0; i < bulletPowers.size(); i++) {
			if (bulletPowers[i]->getColorImportance() > highest) {
				highest = bulletPowers[i]->getColorImportance();
			}
		}

		std::vector<ColorValueHolder> mixingColors; mixingColors.reserve(bulletPowers.size());
		for (int i = 0; i < bulletPowers.size(); i++) {
			if (bulletPowers[i]->getColorImportance() == highest) {
				mixingColors.push_back(bulletPowers[i]->getColor());
			}
		}
		return ColorCacheBullet::insertColor(m_colorIdentifier, mixingColors.data(), mixingColors.size());
	}
}

void Bullet::draw() const {
	const GameSettings& game_settings = GameManager::get_settings();

	if (game_settings.Bullet_PerformanceMode) {
		drawDeathBar();
		drawBody();
	} else {
		drawDeathCooldown();
		drawBody();
		drawOutline();
	}
}

void Bullet::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Bullet::draw!" << std::endl;
			[[fallthrough]];
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
	//don't bother checking for Bullet.PerformanceMode
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
			[[fallthrough]];
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
	const GameSettings& game_settings = GameManager::get_settings();

	if (game_settings.Bullet_PerformanceMode) {
		drawDeathBar(alpha);
		drawBody(alpha);
	} else {
		drawDeathCooldown(alpha);
		drawBody(alpha);
		drawOutline(alpha);
	}
}

void Bullet::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Bullet::ghostDraw!" << std::endl;
			[[fallthrough]];
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

	float coordsAndColor[(Bullet::BulletSideCount+1)*(2+4)];
	coordsAndColor[0] = x;
	coordsAndColor[1] = y;
	coordsAndColor[2] = color.getRf();
	coordsAndColor[3] = color.getGf();
	coordsAndColor[4] = color.getBf();
	coordsAndColor[5] = color.getAf();
	for (int i = 1; i < Bullet::BulletSideCount+1; i++) {
		coordsAndColor[i*6]   = static_cast<float>(x) + static_cast<float>(r) * body_vertices[i].getXComp();
		coordsAndColor[i*6+1] = static_cast<float>(y) + static_cast<float>(r) * body_vertices[i].getYComp();
		coordsAndColor[i*6+2] = color.getRf();
		coordsAndColor[i*6+3] = color.getGf();
		coordsAndColor[i*6+4] = color.getBf();
		coordsAndColor[i*6+5] = color.getAf();
	}

	Renderer::SubmitBatchedDraw(coordsAndColor, (Bullet::BulletSideCount+1)*(2+4), body_indices, Bullet::BulletSideCount*3);
}

inline void Bullet::drawOutline(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 0.5f;

	float coordsAndColor[(Bullet::BulletSideCount*2)*(2+4)];
	for (int i = 0; i < Bullet::BulletSideCount; i++) {
		coordsAndColor[(i*2)  *6]   = static_cast<float>(x) + (static_cast<float>(r)-lineWidth) * body_vertices[i+1].getXComp();
		coordsAndColor[(i*2)  *6+1] = static_cast<float>(y) + (static_cast<float>(r)-lineWidth) * body_vertices[i+1].getYComp();
		coordsAndColor[(i*2+1)*6]   = static_cast<float>(x) + (static_cast<float>(r)+lineWidth) * body_vertices[i+1].getXComp();
		coordsAndColor[(i*2+1)*6+1] = static_cast<float>(y) + (static_cast<float>(r)+lineWidth) * body_vertices[i+1].getYComp();

		coordsAndColor[(i*2)  *6+2] = color.getRf();
		coordsAndColor[(i*2)  *6+3] = color.getGf();
		coordsAndColor[(i*2)  *6+4] = color.getBf();
		coordsAndColor[(i*2)  *6+5] = color.getAf();
		coordsAndColor[(i*2+1)*6+2] = color.getRf();
		coordsAndColor[(i*2+1)*6+3] = color.getGf();
		coordsAndColor[(i*2+1)*6+4] = color.getBf();
		coordsAndColor[(i*2+1)*6+5] = color.getAf();
	}

	Renderer::SubmitBatchedDraw(coordsAndColor, (Bullet::BulletSideCount*2)*(2+4), outline_indices, Bullet::BulletSideCount*6);
}

inline void Bullet::drawDeathCooldown(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	//checking glIsEnabled(GL_BLEND) to skip is an option (though the result should be stored somewhere to avoid GL calls)

	if (this->lifeValue < 100) {
		float deathPercent = std::max(this->lifeValue/100, 0.0f);
		unsigned int deathTriangles = Bullet::BulletSideCount * deathPercent;

		if (deathTriangles > 0) {
			ColorValueHolder color = ColorValueHolder(1.0f, 1.0f, 1.0f);
			color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

			float coordsAndColor[(Bullet::BulletSideCount+1)*(2+4)];
			coordsAndColor[0] = x;
			coordsAndColor[1] = y;
			coordsAndColor[2] = color.getRf();
			coordsAndColor[3] = color.getGf();
			coordsAndColor[4] = color.getBf();
			coordsAndColor[5] = color.getAf();
			for (unsigned int i = 0; i <= deathTriangles && i < Bullet::BulletSideCount; i++) {
				//the final triangle shares its last vertex with the first triangle, which is why this loop condition is a bit strange (second conditional only false for that last triangle)
				//with wrong condition: two verts on an old bullet's death outline to the center of a new bullet's center body or death outline, though sometimes even a tank or rarely the bottom left corner (why)
				//to be more specific: with the old conditional, think it was happening when deathTriangles==1, leading to pushing only two total verts but pushing three indices; but that would mean it was always pushing insufficient verts for the indices, why wasn't it showing up before?

				SimpleVector2D vertex = SimpleVector2D(body_vertices[i+1]);
				//this is a way to hide the lack of vertices: rotate the last vertex to the exact percent it should be
				//requires std::ceil for deathTriangles, and to look correct it requires an additional vertex when nearly full (since it's shared with the last vertex)
				/*
				if (i == deathTriangles) {
					float lastTriangleAngleDelta = float(2*PI) * std::fmod(1-deathPercent, 1.0f/Bullet::BulletSideCount);
					vertex.scaleAndRotate((static_cast<float>(r)+2)*(9.0f/8.0f), -1 * lastTriangleAngleDelta);
				} else {
					vertex.multiplyMagnitude((static_cast<float>(r)+2)*(9.0f/8.0f));
				}
				*/

				coordsAndColor[(i+1)*6]   = static_cast<float>(x) + vertex.getXComp() * (static_cast<float>(r)+2)*(9.0f/8.0f);
				coordsAndColor[(i+1)*6+1] = static_cast<float>(y) + vertex.getYComp() * (static_cast<float>(r)+2)*(9.0f/8.0f);
				coordsAndColor[(i+1)*6+2] = color.getRf();
				coordsAndColor[(i+1)*6+3] = color.getGf();
				coordsAndColor[(i+1)*6+4] = color.getBf();
				coordsAndColor[(i+1)*6+5] = color.getAf();
			}

			Renderer::SubmitBatchedDraw(coordsAndColor, (deathTriangles < Bullet::BulletSideCount ? (deathTriangles+2)*(2+4) : (deathTriangles+1)*(2+4)), body_indices, deathTriangles*3);
		}
	}
}

inline void Bullet::drawDeathBar(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	if (this->lifeValue < 100) {
		float deathPercent = std::max(this->lifeValue/100, 0.0f);

		//fill:

		ColorValueHolder color_extra = ColorValueHolder(1.0f, 1.0f, 1.0f);
		color_extra = ColorMixer::mix(BackgroundRect::getBackColor(), color_extra, alpha);

		float coordsAndColor_extra[] = {
			(this->x - this->r),                            (this->y - this->r*(6.0/4)),   color_extra.getRf(), color_extra.getGf(), color_extra.getBf(), color_extra.getAf(), //0
			(this->x - this->r + deathPercent*(2*this->r)), (this->y - this->r*(6.0/4)),   color_extra.getRf(), color_extra.getGf(), color_extra.getBf(), color_extra.getAf(), //1
			(this->x - this->r + deathPercent*(2*this->r)), (this->y - this->r*(5.0/4)),   color_extra.getRf(), color_extra.getGf(), color_extra.getBf(), color_extra.getAf(), //2
			(this->x - this->r),                            (this->y - this->r*(5.0/4)),   color_extra.getRf(), color_extra.getGf(), color_extra.getBf(), color_extra.getAf()  //3
		};
		unsigned int indices_extra[] = {
			0, 1, 2,
			2, 3, 0,
		};

		Renderer::SubmitBatchedDraw(coordsAndColor_extra, 4 * (2+4), indices_extra, 2 * 3);

		//outline (from Diagnostics):

		ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
		color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
		const float lineWidth = 0.5f;

		float coordsAndColor[] = {
			//outer
			(this->x - this->r) - lineWidth, (this->y - this->r*(6.0/4)) - lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //0
			(this->x + this->r) + lineWidth, (this->y - this->r*(6.0/4)) - lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //1
			(this->x + this->r) + lineWidth, (this->y - this->r*(5.0/4)) + lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //2
			(this->x - this->r) - lineWidth, (this->y - this->r*(5.0/4)) + lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //3

			//inner
			(this->x - this->r),             (this->y - this->r*(6.0/4)),               color.getRf(), color.getGf(), color.getBf(), color.getAf(), //4
			(this->x + this->r),             (this->y - this->r*(6.0/4)),               color.getRf(), color.getGf(), color.getBf(), color.getAf(), //5
			(this->x + this->r),             (this->y - this->r*(5.0/4)),               color.getRf(), color.getGf(), color.getBf(), color.getAf(), //6
			(this->x - this->r),             (this->y - this->r*(5.0/4)),               color.getRf(), color.getGf(), color.getBf(), color.getAf()  //7
		};
		unsigned int indices[] = { //trapezoids
			//bottom
			0, 1, 5,
			5, 4, 0,

			//right
			1, 2, 6,
			6, 5, 1,

			//left
			4, 7, 3,
			3, 0, 4,

			//top
			2, 3, 7,
			7, 6, 2
		};

		Renderer::SubmitBatchedDraw(coordsAndColor, (4+4) * (2+4), indices, (4*2) * 3);
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

BulletUpdateStruct::BulletUpdateStruct(double x, double y, double r, float speed, float angle, float alpha, const std::vector<Bullet*>& newBullets) {
	//add acceleration?
	this->x = x;
	this->y = y;
	this->r = r;
	this->speed = speed;
	this->angle = angle;
	this->alpha = alpha;
	this->newBullets = std::vector<Bullet*>(newBullets);
}

BulletUpdateStruct::BulletUpdateStruct(double x, double y, double r, float speed, float angle, float alpha) {
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
