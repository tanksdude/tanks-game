#include "dev-no-walls-level-1.h"
#include "constants.h"
#include "random-level.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "hazard-manager.h"
#include "reset-things.h"
#include "level-manager.h"

ColorValueHolder DevNoWallsLevel1::getDefaultColor() const {
	//no walls
	return ColorValueHolder(0.0f, 0.0f, 0.0f);
}

std::unordered_map<std::string, float> DevNoWallsLevel1::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", .5f });
	weights.insert({ "random-dev", .5f });
	return weights;
}

void DevNoWallsLevel1::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	//int tempRand;
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;

	PositionHolder* wallArray = new PositionHolder[4];
	for (int i = 0; i < 4; i++) {
		//classic JS walls, except NoBulletZones
		wallArray[i] = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		posArr = new double[4]{ wallArray[i].x, wallArray[i].y, 32, 128 };
		constructionData = GenericFactoryConstructionData(4, posArr);
		HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(constructionData));
		delete[] posArr;
	}

	const double distFromCorner = 40;
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - ((wallArray[0].x+32) + distFromCorner), GAME_HEIGHT/2 - distFromCorner);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "shield"));
	}

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "dev", "inversion")); //this is evil (allowed because it's a dev level)
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "wallhack")); //heh

	//other potential powers:
	//triple, grenade, fire, mines, tracking?
	//other potential hazards:
	//none
}

Level* DevNoWallsLevel1::factory() {
	return new DevNoWallsLevel1();
}

DevNoWallsLevel1::DevNoWallsLevel1() {
	GenericFactoryConstructionData constructionData;
	effects.push_back(LevelManager::getLevelEffectFactory("vanilla", "respawning_powerups")(constructionData));
	//should probably only affect the shield powerups, not everything, but whatever
}
