#include "big-fun-level.h"
#include "constants.h"
#include "level-helper.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "mylib.h" //weightedSelect
#include "rng.h"
#include "reset-things.h"

std::unordered_map<std::string, float> BigFunLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 2.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

ColorValueHolder BigFunLevel::getDefaultColor() const {
	return ColorValueHolder(0.75f, 0.25f, 0.5f);
}

void BigFunLevel::initialize() {
	ResetThings::tankPositionReset(40);

	ColorValueHolder color = getDefaultColor();
	int tempRand;
	PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;

	LevelHelper::pushClassicWalls(color);

	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(LevelHelper::makeNewRandomWall(80+32+40, 40, GAME_WIDTH - 2*(80+32+40), GAME_HEIGHT - 2*40, color));
	}

	//classic speed/invincible/wallhack
	tempRand = RNG::randFunc() * 2;
	pos = LevelHelper::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2 - 16);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla", LevelHelper::simplePowerAlternate(0, tempRand, "speed", "invincible")));
	pos = LevelHelper::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2 - 16);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla", LevelHelper::simplePowerAlternate(1, tempRand, "speed", "invincible")));

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "vanilla", "wallhack"));

	//regular powers in the corners
	std::string possiblePowers[] = { "speed", "wallhack", "bounce", "multishot", "big", "shotgun" }; //6
	//TODO: maybe remove shotgun, maybe remove speed, maybe remove wallhack, maybe add fire
	//barrier may have existed around this time but not only are they not in yet I think the craziness (fun factor) would lower

	float weights[] = { 3.0f, 2.0f, 1.0f };
	for (int i = 0; i < 4; i++) {
		int count = weightedSelect<float>(weights, 3) + 1; //{1, 2, 3}
		std::string* randPowers = LevelHelper::getRandomPowers(count, "random-vanilla", possiblePowers, 6); //TODO: this used to have equal weight; should that be restored?
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+16), GAME_HEIGHT/2-16);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "random-vanilla", randPowers, count));
		delete[] randPowers;
	}
}

Level* BigFunLevel::factory() {
	return new BigFunLevel();
}

BigFunLevel::BigFunLevel() { return; }
