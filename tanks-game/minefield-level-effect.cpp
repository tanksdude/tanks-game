#include "minefield-level-effect.h"

#include "constants.h"
#include <cmath>
#include "rng.h"

#include "bullet-manager.h"
#include "mines-power.h"

std::unordered_map<std::string, float> MinefieldLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	return weights;
}

Bullet* MinefieldLevelEffect::genMine() const {
	//teamID = HAZARD_TEAM
	std::vector<BulletPower*> bp = {(BulletPower*)(new MinesBulletPower())};
	double r = TANK_RADIUS * BULLET_TO_TANK_RADIUS_RATIO;
	double x = minefield_startX + r + RNG::randFunc() * (minefield_areaWidth - 2*r);
	double y = minefield_startY + r + RNG::randFunc() * (minefield_areaHeight - 2*r);
	Bullet* mine = new Bullet(x, y, r, RNG::randFunc() * (2*PI), 0, HAZARD_TEAM, BulletParentType::team, NO_PARENT, &bp);
	return mine;
}

inline void MinefieldLevelEffect::pushRandomMine() const {
	BulletManager::pushBullet(genMine());
}

void MinefieldLevelEffect::apply() {
	for (int i = 0; i < initialMineCount; i++) {
		pushRandomMine();
	}
}

void MinefieldLevelEffect::tick(const Level* parent) {
	if (BulletManager::getNumBullets() >= maxNumOfMines) {
		tickCount = 0;
		return;
	}

	tickCount++;
	if (tickCount >= tickCycle) {
		tickCount -= tickCycle;
		BulletManager::pushBullet(ghostMine);
		ghostMine = genMine();
	}
}

void MinefieldLevelEffect::doEffects(Level* parent) {
	//nothing?
	//might make sense to have the ghost bullet inserting here
}

void MinefieldLevelEffect::draw() const {
	if (BulletManager::getNumBullets() < maxNumOfMines) {
		ghostMine->ghostDraw(tickCount / tickCycle);
	}
}

void MinefieldLevelEffect::draw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for MinefieldLevelEffect::draw!" << std::endl;
		case DrawingLayers::under:
			draw(); //should it be in effects? JS had it there
			break;

		case DrawingLayers::normal:
			//nothing
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

void MinefieldLevelEffect::poseDraw() const {
	//TODO: I have no idea what this should be
	draw();
}

void MinefieldLevelEffect::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for MinefieldLevelEffect::poseDraw!" << std::endl;
		case DrawingLayers::under:
			poseDraw();
			break;

		case DrawingLayers::normal:
			//nothing
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

void MinefieldLevelEffect::ghostDraw(float alpha) const {
	//TODO: this fine?
	ghostMine->ghostDraw(alpha);
}

void MinefieldLevelEffect::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for MinefieldLevelEffect::ghostDraw!" << std::endl;
		case DrawingLayers::under:
			ghostDraw(alpha);
			break;

		case DrawingLayers::normal:
			//nothing
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

MinefieldLevelEffect::MinefieldLevelEffect(double x_start, double y_start, double area_width, double area_height, int initialMineCount) {
	tickCount = 0;
	tickCycle = 400; //in JS, 4*powerSpeed

	minefield_startX = x_start;
	minefield_startY = y_start;
	minefield_areaWidth = area_width;
	minefield_areaHeight = area_height;
	this->initialMineCount = initialMineCount;

	maxNumOfMines = 128; //TODO
	ghostMine = genMine();
}

MinefieldLevelEffect::MinefieldLevelEffect(double x_start, double y_start, double area_width, double area_height) :
	MinefieldLevelEffect(x_start, y_start, area_width, area_height, 16) {}

MinefieldLevelEffect::MinefieldLevelEffect() : MinefieldLevelEffect(0, 0, GAME_WIDTH, GAME_HEIGHT) {}

MinefieldLevelEffect::~MinefieldLevelEffect() {
	delete ghostMine;
}

LevelEffect* MinefieldLevelEffect::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 4) {
			const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
			double x_start = arr[0];
			double y_start = arr[1];
			double area_width = arr[2];
			double area_height = arr[3];

			if ((args.getDataCount() >= 2) && (args.getDataPortionLength(1) >= 1)) {
				const int* arr_mine = static_cast<const int*>(args.getDataPortion(1).get());
				int mineCount = arr_mine[0];
				return new MinefieldLevelEffect(x_start, y_start, area_width, area_height, mineCount);
			}

			return new MinefieldLevelEffect(x_start, y_start, area_width, area_height);
		}
	}
	return new MinefieldLevelEffect();
}
