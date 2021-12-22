#include "big-fun-level.h"
#include "constants.h"
#include "random-level.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "mylib.h"
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
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	ColorValueHolder color = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	//std::string* paras;

	for (int i = 0; i < 4; i++) {
		//classic JS walls
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}

	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(RandomLevel::makeNewRandomWall(80+32+40, 40, GAME_WIDTH - 2*(80+32+40), GAME_HEIGHT - 2*40, color));
	}

	std::string possiblePowers[] = { "speed", "invincible", "wallhack", "bounce", "big", "multishot" }; //6
	//include invincible?
	//barrier and shotgun may have existed around this time but not only are they not in yet I think the craziness (fun factor) would lower

	//get attributes (actually just whether it can stack) of the powers
	bool* canStack = new bool[6];
	for (int i = 0; i < 6; i++) {
		Power* p = PowerupManager::getPowerFactory("vanilla", possiblePowers[i])();
		std::vector<std::string> attributes = p->getPowerAttributes();
		canStack[i] = (std::find(attributes.begin(), attributes.end(), "stack") != attributes.end());
		delete p;
	}

	float weights[] = { 2.0f, 2.0f, 1.0f };
	for (int i = 0; i < 4; i++) {
		int count = weightedSelect<float>(weights, 3) + 1; //{1, 2, 3}
		std::string* randPowers = RandomLevel::getRandomPowers(count, canStack, possiblePowers, 6);
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+16), GAME_HEIGHT/2-16);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, randPowers, count));
		delete[] randPowers;
	}
	delete[] canStack;
}

Level* BigFunLevel::factory() {
	return new BigFunLevel();
}

BigFunLevel::BigFunLevel() { return; }
