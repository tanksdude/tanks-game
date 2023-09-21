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

inline void GinormousTurretHazard::MinionTurret::drawReticule(float alpha) const {
	if (currentState == 2) {
		//this is an absolute hack
		return;
	}
	TargetingTurretHazard::drawReticule(alpha);
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
