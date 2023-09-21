#include "ginormous-turret-hazard.h"

#include "../constants.h"
#include <cmath>
#include <algorithm> //std::clamp
#include <iostream>
#include "../rng.h"

#include "../renderer.h"
#include "../color-mixer.h"
#include "../background-rect.h"

#include "../tank.h"
#include "../tank-manager.h"
#include "../bullet-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

std::unordered_map<std::string, float> GinormousTurretHazard::MinionTurret::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 0.0f });
	weights.insert({ "dev", 0.0f });
	return weights;
}

GinormousTurretHazard::MinionTurret::MinionTurret(double xpos, double ypos, double angle, Game_ID parentID) : TargetingTurretHazard(xpos, ypos, angle) {
	this->parentID = parentID;
}

GinormousTurretHazard::MinionTurret::~MinionTurret() {
	//nothing
}

CircleHazard* GinormousTurretHazard::MinionTurret::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 2) {
		int count = args.getDataPortionLength(0);
		int count_id = args.getDataPortionLength(1);

		if (count >= 3 && count_id >= 1) {
			const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
			const Game_ID* arr_id = static_cast<const Game_ID*>(args.getDataPortion(1).get());
			double x = arr[0];
			double y = arr[1];
			double a = arr[2];
			Game_ID parent = arr_id[0];
			return new MinionTurret(x, y, a, parent);
		}
	}
	return new MinionTurret(0, 0, 0, 0);
}

void GinormousTurretHazard::MinionTurret::setTarget(Game_ID parentVerification, Game_ID target) {
	if (this->parentID == parentVerification) {
		this->trackingID = target;
		this->targeting = true;
	}
}

void GinormousTurretHazard::MinionTurret::unsetTarget(Game_ID parentVerification) {
	if (this->parentID == parentVerification) {
		this->trackingID = this->getGameID();
		this->targeting = false;
	}
}

void GinormousTurretHazard::MinionTurret::tick() {
	if (currentState == 0) { //either tracking a tank or doing nothing
		if (targeting) { //tracking tank
			tick_continueTracking();
		} else { //no orders
			targetingCount = 0;
			return;
		}
	}

	if (currentState == 1) { //charging up to shoot (and shooting)
		tick_chargeUp();
	}

	if (currentState == 2) { //cooling down
		tick_cooldown();
	}
}

bool GinormousTurretHazard::MinionTurret::canSeeTank(const Tank* t) const {
	return (this->trackingID == t->getGameID());
}

void GinormousTurretHazard::MinionTurret::draw() const {
	drawBody();
	drawOutline();
	drawBarrel();
	drawReticule();
}

void GinormousTurretHazard::MinionTurret::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for GinormousTurretHazard::MinionTurret::draw!" << std::endl;
		case DrawingLayers::normal:
			drawBody();
			drawOutline();
			drawBarrel();
			break;

		case DrawingLayers::effects:
			drawReticule();
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void GinormousTurretHazard::MinionTurret::poseDraw() const {
	drawBody();
	drawOutline();
	drawBarrel();
}

void GinormousTurretHazard::MinionTurret::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for GinormousTurretHazard::MinionTurret::poseDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBody();
			drawOutline();
			drawBarrel();
			break;

		case DrawingLayers::effects:
			//drawReticule();
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void GinormousTurretHazard::MinionTurret::ghostDraw(float alpha) const {
	drawBody(alpha);
	drawOutline(alpha);
	drawBarrel(alpha);
}

void GinormousTurretHazard::MinionTurret::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for GinormousTurretHazard::MinionTurret::ghostDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBody(alpha);
			drawOutline(alpha);
			drawBarrel(alpha);
			break;

		case DrawingLayers::effects:
			//drawReticule(alpha);
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

inline void GinormousTurretHazard::MinionTurret::drawBody(float alpha) const {
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

inline void GinormousTurretHazard::MinionTurret::drawOutline(float alpha) const {
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

inline void GinormousTurretHazard::MinionTurret::drawBarrel(float alpha) const {
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

inline void GinormousTurretHazard::MinionTurret::drawReticule(float alpha) const {
	if (!targeting || currentState == 2) {
		//this is an absolute hack
		return;
	}

	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color_outline = getReticuleColor();
	color_outline = ColorMixer::mix(BackgroundRect::getBackColor(), color_outline, alpha);
	const float lineWidth = 1.0f;
	//optional rotate angle: (-PI/2) * (targetingCount/(stateMultiplier[0] * tickCycle))

	float coordsAndColor_outline[(Circle::numOfSides*2 + 4*4*2)*(2+4)];
	for (int i = 0; i < Circle::numOfSides; i++) {
		coordsAndColor_outline[(i*2)  *6]   = targetingX + ((2*TANK_RADIUS) - lineWidth) * cos(i * (2*PI / Circle::numOfSides));
		coordsAndColor_outline[(i*2)  *6+1] = targetingY + ((2*TANK_RADIUS) - lineWidth) * sin(i * (2*PI / Circle::numOfSides));
		coordsAndColor_outline[(i*2+1)*6]   = targetingX + ((2*TANK_RADIUS) + lineWidth) * cos(i * (2*PI / Circle::numOfSides));
		coordsAndColor_outline[(i*2+1)*6+1] = targetingY + ((2*TANK_RADIUS) + lineWidth) * sin(i * (2*PI / Circle::numOfSides));

		coordsAndColor_outline[(i*2)  *6+2] = color_outline.getRf();
		coordsAndColor_outline[(i*2)  *6+3] = color_outline.getGf();
		coordsAndColor_outline[(i*2)  *6+4] = color_outline.getBf();
		coordsAndColor_outline[(i*2)  *6+5] = color_outline.getAf();
		coordsAndColor_outline[(i*2+1)*6+2] = color_outline.getRf();
		coordsAndColor_outline[(i*2+1)*6+3] = color_outline.getGf();
		coordsAndColor_outline[(i*2+1)*6+4] = color_outline.getBf();
		coordsAndColor_outline[(i*2+1)*6+5] = color_outline.getAf();
	}

	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0))  *6]   = targetingX + ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0))  *6+1] = targetingY - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+1))*6]   = targetingX + (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+1))*6+1] = targetingY - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+2))*6]   = targetingX + (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+2))*6+1] = targetingY + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+3))*6]   = targetingX + ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+3))*6+1] = targetingY + lineWidth;

	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1))  *6]   = targetingX + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1))  *6+1] = targetingY + ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+1))*6]   = targetingX + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+1))*6+1] = targetingY + (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+2))*6]   = targetingX - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+2))*6+1] = targetingY + (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+3))*6]   = targetingX - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+3))*6+1] = targetingY + ( .75*(2*TANK_RADIUS) - lineWidth);

	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2))  *6]   = targetingX - ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2))  *6+1] = targetingY - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+1))*6]   = targetingX - (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+1))*6+1] = targetingY - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+2))*6]   = targetingX - (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+2))*6+1] = targetingY + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+3))*6]   = targetingX - ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+3))*6+1] = targetingY + lineWidth;

	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3))  *6]   = targetingX - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3))  *6+1] = targetingY - ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+1))*6]   = targetingX - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+1))*6+1] = targetingY - (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+2))*6]   = targetingX + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+2))*6+1] = targetingY - (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+3))*6]   = targetingX + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+3))*6+1] = targetingY - ( .75*(2*TANK_RADIUS) - lineWidth);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			coordsAndColor_outline[(Circle::numOfSides*2 + (4*i+j))*6+2] = color_outline.getRf();
			coordsAndColor_outline[(Circle::numOfSides*2 + (4*i+j))*6+3] = color_outline.getGf();
			coordsAndColor_outline[(Circle::numOfSides*2 + (4*i+j))*6+4] = color_outline.getBf();
			coordsAndColor_outline[(Circle::numOfSides*2 + (4*i+j))*6+5] = color_outline.getAf();
		}
	}

	unsigned int indices_outline[Circle::numOfSides*6 + 4*6];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices_outline[i*6]   =  i*2;
		indices_outline[i*6+1] =  i*2+1;
		indices_outline[i*6+2] = (i*2+3) % (Circle::numOfSides*2);
		indices_outline[i*6+3] = (i*2+3) % (Circle::numOfSides*2);
		indices_outline[i*6+4] = (i*2+2) % (Circle::numOfSides*2);
		indices_outline[i*6+5] =  i*2;
	}

	for (int i = 0; i < 4; i++) {
		indices_outline[Circle::numOfSides*6 + i*6]   = (Circle::numOfSides*2 + 4*i);
		indices_outline[Circle::numOfSides*6 + i*6+1] = (Circle::numOfSides*2 + 4*i+1);
		indices_outline[Circle::numOfSides*6 + i*6+2] = (Circle::numOfSides*2 + 4*i+2);
		indices_outline[Circle::numOfSides*6 + i*6+3] = (Circle::numOfSides*2 + 4*i+2);
		indices_outline[Circle::numOfSides*6 + i*6+4] = (Circle::numOfSides*2 + 4*i+3);
		indices_outline[Circle::numOfSides*6 + i*6+5] = (Circle::numOfSides*2 + 4*i);
	}

	Renderer::SubmitBatchedDraw(coordsAndColor_outline, (Circle::numOfSides*2 + 4*4*2)*(2+4), indices_outline, Circle::numOfSides*6 + 4*6);
}



std::unordered_map<std::string, float> GinormousTurretHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 0.25f });
	weights.insert({ "random-vanilla", 0.125f });
	weights.insert({ "random", 0.0625f });
	return weights;
}

GinormousTurretHazard::GinormousTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren, double childDistMultiplier) : MotherTurretHazard(xpos, ypos, angle, maxChildren, startChildren, childDistMultiplier) {
	//x = xpos;
	//y = ypos;
	//velocity = SimpleVector2D(angle, 0, true);
	r = TANK_RADIUS * 4; // MotherTurretHazard radius * 2 //my vision for this turret was twice as large as this value, but that can barely fit on the screen
	//future note: my idea for the "god turret" is r = TANK_RADIUS * 64 or 32 (32 might be better for gameplay, 64 is more what I wanted), which is nowhere near being able to fit on the screen; since it's so big it doesn't even recognize the little tanks
	//lore idea (this is really stupid): since there are turret sizes smaller and larger than the tanks, but not of the same size, does that imply the tanks are turrets (that gained sentience)? what a plot twist!
	childDistFromMother = r * childDistMultiplier;

	stateMultiplier[1] /= 2;
	//turningIncrement /= 2;

	//canAcceptPowers = false;
}

GinormousTurretHazard::GinormousTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren) : GinormousTurretHazard(xpos, ypos, angle, maxChildren, startChildren, .25) {
	//nothing
}

GinormousTurretHazard::GinormousTurretHazard(double xpos, double ypos, double angle) : GinormousTurretHazard(xpos, ypos, angle, 16, 8) {
	//nothing
}

GinormousTurretHazard::~GinormousTurretHazard() {
	//nothing
}

CircleHazard* GinormousTurretHazard::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 3) {
			const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
			double x = arr[0];
			double y = arr[1];
			double a = arr[2];
			return new GinormousTurretHazard(x, y, a);
		}
	}
	return new GinormousTurretHazard(0, 0, 0);
}

void GinormousTurretHazard::turnTowardsTank(const Tank* t) {
	TargetingTurretHazard::turnTowardsTank(t);
	TargetingTurretHazard::turnTowardsTank(t);
	//this is hacky
}

inline CircleHazard* GinormousTurretHazard::makeTurret(int turretNum) const {
	GenericFactoryConstructionData constructionData;
	double angle = getChildTurretAngle(turretNum);
	double* posArr = new double[3]{ this->x + (this->r+childDistFromMother) * cos(angle), this->y + (this->r+childDistFromMother) * sin(angle), angle };
	constructionData = GenericFactoryConstructionData(3, posArr);
	Game_ID* id = new Game_ID[1]{ this->getGameID() };
	constructionData.pushData(1, id);
	CircleHazard* childTurret = MinionTurret::factory(constructionData);
	return childTurret;
}

void GinormousTurretHazard::tick() {
	updateChildCount();
	if (getChildCount() < maxChildTurrets/4) {
		//behave like a MotherTurret
		//tick_stopChildren(); //probably fine for the children to keep targeting, they just won't receive new orders
		MotherTurretHazard::tick();
	} else {
		//behave like a MotherTurret when there's no target
		//otherwise behave sorta like a TargetingTurret

		if (currentState == 0) {
			if (targeting) { //tracking tank
				tick_continueTracking();
				targetingCount = 0;
			}
			if (!targeting) { //looking for a tank
				tick_lookForNewTarget();
			}

			if (targeting) {
				tick_notifyChildren(trackingID);
			} else {
				tick_stopChildren();
				MotherTurretHazard::tick();
			}
		}

		if (currentState == 1) { //charging up
			tick_chargeUp();
		}

		if (currentState == 2) { //cooling down
			tick_cooldown();
		}
	}
}

void GinormousTurretHazard::tick_notifyChildren(Game_ID id) {
	for (int i = 0; i < childTurretIDs.size(); i++) {
		if (!childTurretAlive[i]) {
			continue;
		}
		CircleHazard* ch = HazardManager::getCircleHazardByID(childTurretIDs[i]);
		if (ch != nullptr) { //sanity check
			MinionTurret* mt = static_cast<MinionTurret*>(ch);
			mt->setTarget(this->getGameID(), id);
		}
	}
}

void GinormousTurretHazard::tick_stopChildren() {
	for (int i = 0; i < childTurretIDs.size(); i++) {
		if (!childTurretAlive[i]) {
			continue;
		}
		CircleHazard* ch = HazardManager::getCircleHazardByID(childTurretIDs[i]);
		if (ch != nullptr) { //sanity check
			MinionTurret* mt = static_cast<MinionTurret*>(ch);
			mt->unsetTarget(this->getGameID());
		}
	}
}

void GinormousTurretHazard::draw() const {
	drawShootingTimer();
	drawBody();
	drawOutline();
	drawBarrel();
}

void GinormousTurretHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for GinormousTurretHazard::draw!" << std::endl;
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

void GinormousTurretHazard::poseDraw() const {
	drawShootingTimer(); //TODO: should this happen?
	drawBody();
	drawOutline();
	drawBarrel();
}

void GinormousTurretHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for GinormousTurretHazard::poseDraw!" << std::endl;
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

void GinormousTurretHazard::ghostDraw(float alpha) const {
	drawShootingTimer(alpha); //TODO: should this happen?
	drawBody(alpha);
	drawOutline(alpha);
	drawBarrel(alpha);
}

void GinormousTurretHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for GinormousTurretHazard::ghostDraw!" << std::endl;
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

inline void GinormousTurretHazard::drawBody(float alpha) const {
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

inline void GinormousTurretHazard::drawOutline(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 2.0f;

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

inline void GinormousTurretHazard::drawBarrel(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 3.0f;

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

inline void GinormousTurretHazard::drawShootingTimer(float alpha) const {
	if (currentState != 1) {
		return;
	}

	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	double shootingOutlinePercent;
	if (tickCycle <= 0) {
		shootingOutlinePercent = 0;
	} else {
		shootingOutlinePercent = std::clamp<double>(targetingCount / (stateMultiplier[1] * tickCycle), 0, 1);
	}
	unsigned int shootingOutlineTriangles = Circle::numOfSides * shootingOutlinePercent;

	if (shootingOutlineTriangles > 0) {
		ColorValueHolder color = ColorValueHolder(1.0f, 1.0f, 1.0f);
		color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
		double rotateAngle = velocity.getAngle() + (2*PI)*(1 - double(shootingOutlineTriangles)/Circle::numOfSides)/2;

		float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
		coordsAndColor[0] = x;
		coordsAndColor[1] = y;
		coordsAndColor[2] = color.getRf();
		coordsAndColor[3] = color.getGf();
		coordsAndColor[4] = color.getBf();
		coordsAndColor[5] = color.getAf();
		for (unsigned int i = 0; i <= shootingOutlineTriangles && i < Circle::numOfSides; i++) {
			coordsAndColor[(i+1)*6]   = x + (r*(5.0/4.0)) * cos(rotateAngle + i * (2*PI / Circle::numOfSides));
			coordsAndColor[(i+1)*6+1] = y + (r*(5.0/4.0)) * sin(rotateAngle + i * (2*PI / Circle::numOfSides));
			coordsAndColor[(i+1)*6+2] = color.getRf();
			coordsAndColor[(i+1)*6+3] = color.getGf();
			coordsAndColor[(i+1)*6+4] = color.getBf();
			coordsAndColor[(i+1)*6+5] = color.getAf();
		}

		unsigned int indices[Circle::numOfSides*3];
		for (unsigned int i = 0; i < shootingOutlineTriangles; i++) {
			indices[i*3]   = 0;
			indices[i*3+1] = i+1;
			indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
		}

		Renderer::SubmitBatchedDraw(coordsAndColor, (shootingOutlineTriangles < Circle::numOfSides ? (shootingOutlineTriangles+2)*(2+4) : (shootingOutlineTriangles+1)*(2+4)), indices, shootingOutlineTriangles*3);
	}
}

CircleHazard* GinormousTurretHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData& args) {
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, angle;

	int count = 0;
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);
	}
	if (count >= 1) {
		const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
		angle = arr[0];
	} else {
		angle = RNG::randFunc() * (2*PI);
	}

	do {
		xpos = RNG::randFunc() * (area_width - 2*TANK_RADIUS/4) + (x_start + TANK_RADIUS/4);
		ypos = RNG::randFunc() * (area_height - 2*TANK_RADIUS/4) + (y_start + TANK_RADIUS/4);
		CircleHazard* testGinormousTurret = new GinormousTurretHazard(xpos, ypos, angle);
		if (testGinormousTurret->reasonableLocation()) {
			randomized = testGinormousTurret;
			break;
		} else {
			delete testGinormousTurret;
		}
		attempts++;
	} while (attempts < 128);

	return randomized;
}
