#include "bullet.h"
#include "game-manager.h"
#include "mylib.h"
#include "constants.h"
#include <cmath>
#include "color-mixer.h"
#include "renderer.h"
#include "background-rect.h"
#include <iostream>

//for CPU drawing, in case other #includes go wrong:
#include <GL/glew.h>
#include <GL/freeglut.h>

VertexArray* Bullet::va;
VertexBuffer* Bullet::vb;
IndexBuffer* Bullet::ib;
bool Bullet::initialized_GPU = false;

const double Bullet::default_radius = 4;
Bullet::Bullet(double x_, double y_, double angle, Team_ID teamID, BulletParentType parentType, Game_ID parentID) : GameThing(teamID) { //every bullet constructor does this stuff
	initializeGPU();
	this->x = x_;
	this->y = y_;
	this->velocity = SimpleVector2D(angle, 0, true);
	this->parentType = parentType;
	this->parentID = parentID;
	this->opaqueness = 100;
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

	//uninitializeGPU();
}

/*
bool Bullet::isDead() const {
	return (opaqueness <= 0);
}
*/

bool Bullet::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	float positions[(Circle::numOfSides+1)*2];
	positions[0] = 0;
	positions[1] = 0;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		positions[i*2]   = cos((i-1) * 2*PI / Circle::numOfSides);
		positions[i*2+1] = sin((i-1) * 2*PI / Circle::numOfSides);
	}

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = 0;
		indices[i*3+1] = i+1;
		indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	vb = VertexBuffer::MakeVertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		//{ ShaderDataType::Float4, "a_Color" }
	};
	vb->SetLayout(layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	va = VertexArray::MakeVertexArray();
	va->AddVertexBuffer(vb);
	va->SetIndexBuffer(ib);

	initialized_GPU = true;
	return true;
}

bool Bullet::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;

	initialized_GPU = false;
	return true;
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
	this->opaqueness += up->alpha;
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

	double importance = LOW_IMPORTANCE;
	for (int i = 0; i < bulletPowers.size(); i++) {
		double value = bulletPowers[i]->getBulletAccelerationImportance();
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
		if (bulletPowers[i]->isDone()) {
			removePower(i);
		} else { //to make each power last its full length, not n-1 length
			bulletPowers[i]->powerTick(this);
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
		double highest = LOW_IMPORTANCE;
		for (int i = 0; i < bulletPowers.size(); i++) {
			if (bulletPowers[i]->getColorImportance() > highest) {
				highest = bulletPowers[i]->getColorImportance();
			}
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

/*
void Bullet::drawCPU() const {
	drawCPU(x, y);
}

void Bullet::drawCPU(double xpos, double ypos) const {
	//main body:
	ColorValueHolder color = getColor();
	glColor3f(color.getRf(), color.getGf(), color.getBf());

	glBegin(GL_POLYGON);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex3f(xpos + r*cos(i * 2*PI / Circle::numOfSides), ypos + r*sin(i * 2*PI / Circle::numOfSides), 0);
	}

	glEnd();

	//outline:
	glColor3f(0, 0, 0);
	glLineWidth(1);

	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex2f(xpos + r*cos(i * 2*PI / Circle::numOfSides), ypos + r*sin(i * 2*PI / Circle::numOfSides));
	}

	glEnd();
}
*/

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
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	/*
	if (glIsEnabled(GL_BLEND)) {
		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), this->opaqueness/100);
	} else {
		if (this->opaqueness < 100) {
			double deathPercent = constrain<double>(this->opaqueness/100, 0, 1);
			unsigned int deathVertices = Circle::numOfSides * deathPercent;

			if (deathVertices > 0) {
				glm::mat4 modelMatrix_deathOutline = Renderer::GenerateModelMatrix((r+2) * 9.0/8.0, (r+2) * 9.0/8.0, PI/2, x, y);

				shader->setUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);
				shader->setUniformMat4f("u_ModelMatrix", modelMatrix_deathOutline);

				Renderer::Draw(*va, *ib, *shader, deathVertices*3);
			}
		}

		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), 1.0f);
	}
	*/
	ColorValueHolder color = getColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*va, *ib, *shader);
}

inline void Bullet::drawOutline(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f); //lines still look ugly even with glEnable(GL_LINE_SMOOTH), so I don't know what to set it at

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(GL_LINE_LOOP, 1, Circle::numOfSides);

	//cleanup:
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//drawing an outline: use a geometry shader (ugh) or another VAO+IBO (lesser ugh), the CPU (big ugh), or glDrawArrays with GL_LINE_LOOP (yay!)
}

inline void Bullet::drawDeathCooldown(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	//glm::mat4 modelMatrix;

	//if (glIsEnabled(GL_BLEND)) {
	//	//skip
	//} else {
		if (this->opaqueness < 100) {
			double deathPercent = constrain<double>(this->opaqueness/100, 0, 1);
			unsigned int deathVertices = Circle::numOfSides * deathPercent;

			if (deathVertices > 0) {
				ColorValueHolder color = ColorValueHolder(1.0f, 1.0f, 1.0f);
				color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
				shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

				glm::mat4 modelMatrix_deathOutline = Renderer::GenerateModelMatrix((r+2) * 9.0/8.0, (r+2) * 9.0/8.0, PI/2, x, y);
				shader->setUniformMat4f("u_ModelMatrix", modelMatrix_deathOutline);

				Renderer::Draw(*va, *ib, *shader, deathVertices*3);
			}
		}
	//}
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

double Bullet::getHighestOffenseImportance() const {
	double highest = LOW_IMPORTANCE;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getOffenseImportance() > highest) {
			highest = bulletPowers[i]->getOffenseImportance();
		}
	}
	return highest;
}

double Bullet::getHighestOffenseTier(double importance) const {
	double highest = LOW_TIER;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getOffenseImportance() == importance) {
			if (bulletPowers[i]->getOffenseTier(this) > highest) {
				highest = bulletPowers[i]->getOffenseTier(this);
			}
		}
	}
	if (bulletPowers.size() == 0) {
		return 0;
	}
	return highest;
}

double Bullet::getOffenseTier() const {
	return getHighestOffenseTier(getHighestOffenseImportance());
}

double Bullet::getHighestDefenseImportance() const {
	double highest = LOW_IMPORTANCE;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getDefenseImportance() > highest) {
			highest = bulletPowers[i]->getDefenseImportance();
		}
	}
	return highest;
}

double Bullet::getHighestDefenseTier(double importance) const {
	double highest = LOW_TIER;
	for (int i = 0; i < bulletPowers.size(); i++) {
		if (bulletPowers[i]->getDefenseImportance() == importance) {
			if (bulletPowers[i]->getDefenseTier(this) > highest) {
				highest = bulletPowers[i]->getDefenseTier(this);
			}
		}
	}
	if (bulletPowers.size() == 0) {
		return 0;
	}
	return highest;
}

double Bullet::getDefenseTier() const {
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
