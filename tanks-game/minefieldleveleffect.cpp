#include "minefieldleveleffect.h"
#include "constants.h"
#include <math.h>
#include "rng.h"
#include "bulletmanager.h"
#include "minespower.h"

std::unordered_map<std::string, float> MinefieldLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
	return weights;
}

Bullet* MinefieldLevelEffect::genMine() const {
	//teamID = HAZARD_TEAM
	std::vector<BulletPower*> bp = {(BulletPower*)(new MinesBulletPower())};
	double r = TANK_RADIUS * BULLET_TO_TANK_RADIUS_RATIO;
	double x = minefield_startX + r + RNG::randFunc2() * (minefield_areaWidth - 2*r);
	double y = minefield_startY + r + RNG::randFunc2() * (minefield_areaHeight - 2*r);
	Bullet* mine = new Bullet(x, y, r, RNG::randFunc() * 2*PI, 0, HAZARD_TEAM, BulletParentType::team, NO_PARENT, &bp);
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
			draw(); //should it be in effects...? JS had it here
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
	//TODO
}

void MinefieldLevelEffect::poseDraw(DrawingLayers layer) const {
	//TODO
}

void MinefieldLevelEffect::ghostDraw(float alpha) const {
	//TODO
}

void MinefieldLevelEffect::ghostDraw(DrawingLayers layer, float alpha) const {
	//TODO
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

LevelEffect* MinefieldLevelEffect::factory(int argc, std::string* argv) {
	if (argc >= 4) {
		double x_start = std::stoi(argv[0]);
		double y_start = std::stoi(argv[1]);
		double area_width = std::stoi(argv[2]);
		double area_height = std::stoi(argv[3]);
		if (argc >= 5) {
			int mineCount = std::stoi(argv[4]);
			return new MinefieldLevelEffect(x_start, y_start, area_width, area_height, mineCount);
		}
		return new MinefieldLevelEffect(x_start, y_start, area_width, area_height);
	}
	return new MinefieldLevelEffect();
}
