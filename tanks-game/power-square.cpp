#include "power-square.h"
#include "game-manager.h"
#include "constants.h"
#include "background-rect.h"
#include "color-mixer.h"
#include "renderer.h"
#include "powerup-manager.h"
#include "mylib.h"

const double PowerSquare::POWER_WIDTH = 6;
const double PowerSquare::POWER_HEIGHT = 6;
const double PowerSquare::POWER_LINE_WIDTH = 1.0/3.0;
const double PowerSquare::POWER_OUTLINE_MULTIPLIER = 1.5;

PowerSquare::PowerSquare(double x_, double y_) : GameThing(DEFAULT_TEAM) {
	x = x_ - PowerSquare::POWER_WIDTH/2;
	y = y_ - PowerSquare::POWER_HEIGHT/2;
	w = PowerSquare::POWER_WIDTH;
	h = PowerSquare::POWER_HEIGHT;
}

PowerSquare::PowerSquare(double x_, double y_, std::string name) : PowerSquare(x_, y_) {
	numOfPowers = 1;
	heldPowers = new Power*[1];
	heldPowers[0] = PowerupManager::getPowerFactory("vanilla", name)();
}

PowerSquare::PowerSquare(double x_, double y_, std::string* names, int num) : PowerSquare(x_, y_) {
	numOfPowers = num;
	heldPowers = new Power*[num];
	for (int i = 0; i < num; i++) {
		heldPowers[i] = PowerupManager::getPowerFactory("vanilla", names[i])();
	}
}

PowerSquare::PowerSquare(double x_, double y_, std::string type, std::string name) : PowerSquare(x_, y_) {
	numOfPowers = 1;
	heldPowers = new Power*[1];
	heldPowers[0] = PowerupManager::getPowerFactory(type, name)();
}

PowerSquare::PowerSquare(double x_, double y_, std::string type, std::string* names, int num) : PowerSquare(x_, y_) {
	numOfPowers = num;
	heldPowers = new Power*[num];
	for (int i = 0; i < num; i++) {
		heldPowers[i] = PowerupManager::getPowerFactory(type, names[i])();
	}
}

PowerSquare::PowerSquare(double x_, double y_, std::string* types, std::string* names, int num) : PowerSquare(x_, y_) {
	numOfPowers = num;
	heldPowers = new Power*[num];
	for (int i = 0; i < num; i++) {
		heldPowers[i] = PowerupManager::getPowerFactory(types[i], names[i])();
	}
}

PowerSquare::PowerSquare(const PowerSquare& other) : PowerSquare(other.x+PowerSquare::POWER_WIDTH/2, other.y+PowerSquare::POWER_HEIGHT/2) {
	this->numOfPowers = other.numOfPowers;
	this->heldPowers = new Power*[numOfPowers];
	for (int i = 0; i < numOfPowers; i++) {
		heldPowers[i] = PowerupManager::getPowerFactory(other.heldPowers[i]->getPowerTypes()[0], other.heldPowers[i]->getName())();
	}
}

PowerSquare::~PowerSquare() {
	for (int i = 0; i < numOfPowers; i++) {
		delete heldPowers[i];
	}
	delete[] heldPowers;
}

ColorValueHolder PowerSquare::getColor() const {
	if (numOfPowers == 1) {
		return heldPowers[0]->getColor();
	} else {
		double highest = LOW_IMPORTANCE;
		for (int i = 0; i < numOfPowers; i++) {
			if (heldPowers[i]->getColorImportance() > highest) {
				highest = heldPowers[i]->getColorImportance();
			}
		}
		std::vector<ColorValueHolder> mixingColors;
		for (int i = 0; i < numOfPowers; i++) {
			if (heldPowers[i]->getColorImportance() == highest) {
				mixingColors.push_back(heldPowers[i]->getColor());
			}
		}
		return ColorMixer::mix(mixingColors.data(), mixingColors.size());
	}
}

void PowerSquare::givePower(Tank* t) {
	for (int i = 0; i < numOfPowers; i++) {
		t->tankPowers.push_back(heldPowers[i]->makeTankPower());
		t->tankPowers[t->tankPowers.size()-1]->initialize(t);
	}
	t->determineShootingAngles();
	t->updateAllValues();
	//good enough for now
}

//void PowerSquare::givePower(Bullet*);
//void PowerSquare::givePower(Hazard*);

void PowerSquare::draw() const {
	drawOutlineThing();
	drawMain();
}

void PowerSquare::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			//TODO: should drawOutlineThing() be here?
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for PowerSquare::draw!" << std::endl;
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

void PowerSquare::poseDraw() const {
	drawOutlineThing();
	drawMain();
}

void PowerSquare::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for PowerSquare::poseDraw!" << std::endl;
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

void PowerSquare::ghostDraw(float alpha) const {
	drawOutlineThing(alpha);
	drawMain(alpha);
}

void PowerSquare::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for PowerSquare::ghostDraw!" << std::endl;
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

inline void PowerSquare::drawMain(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = getColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

	float coordsAndColor_main[] = {
		//main ring
		x,   y,     color.getRf(), color.getGf(), color.getBf(), color.getAf(), //0
		x+w, y,     color.getRf(), color.getGf(), color.getBf(), color.getAf(), //1
		x+w, y+h,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //2
		x,   y+h,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //3

		//inner ring
		(x)   + (w * PowerSquare::POWER_LINE_WIDTH), (y)   + (h * PowerSquare::POWER_LINE_WIDTH),   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //4
		(x+w) - (w * PowerSquare::POWER_LINE_WIDTH), (y)   + (h * PowerSquare::POWER_LINE_WIDTH),   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //5
		(x+w) - (w * PowerSquare::POWER_LINE_WIDTH), (y+h) - (h * PowerSquare::POWER_LINE_WIDTH),   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //6
		(x)   + (w * PowerSquare::POWER_LINE_WIDTH), (y+h) - (h * PowerSquare::POWER_LINE_WIDTH),   color.getRf(), color.getGf(), color.getBf(), color.getAf()  //7
	};
	unsigned int indices_main[] = { //trapezoids
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

	Renderer::SubmitBatchedDraw(coordsAndColor_main, (4+4) * (2+4), indices_main, (4*2) * 3);
}

inline void PowerSquare::drawOutlineThing(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	if (numOfPowers > 1) {
		const float extendingMultiplier = PowerSquare::POWER_LINE_WIDTH * (PowerSquare::POWER_OUTLINE_MULTIPLIER - 1);
		ColorValueHolder backgroundMix = ColorMixer::mix(getColor(), BackgroundRect::getBackColor());
		backgroundMix = ColorMixer::mix(BackgroundRect::getBackColor(), backgroundMix, alpha);

		float coordsAndColor_outline[] = {
			//outer ring
			(x)   - (w * extendingMultiplier), (y)   - (h * extendingMultiplier),   backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf(), backgroundMix.getAf(), //0
			(x+w) + (w * extendingMultiplier), (y)   - (h * extendingMultiplier),   backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf(), backgroundMix.getAf(), //1
			(x+w) + (w * extendingMultiplier), (y+h) + (h * extendingMultiplier),   backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf(), backgroundMix.getAf(), //2
			(x)   - (w * extendingMultiplier), (y+h) + (h * extendingMultiplier),   backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf(), backgroundMix.getAf(), //3

			//main ring
			x,   y,     backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf(), backgroundMix.getAf(), //4
			x+w, y,     backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf(), backgroundMix.getAf(), //5
			x+w, y+h,   backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf(), backgroundMix.getAf(), //6
			x,   y+h,   backgroundMix.getRf(), backgroundMix.getGf(), backgroundMix.getBf(), backgroundMix.getAf()  //7
		};
		unsigned int indices_outline[] = { //trapezoids
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

		Renderer::SubmitBatchedDraw(coordsAndColor_outline, (4+4) * (2+4), indices_outline, (4*2) * 3);
	}
}
