#include "fewobstacleslevel.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "mylib.h"
#include "resetthings.h"

void FewObstaclesLevel::initialize() {
	int randPos = randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), randPos);

	ColorValueHolder color = ColorValueHolder(.25f, .75f, 0x99/255.0);

	PositionHolder pos = RandomLevel::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20), 20, GAME_HEIGHT-128);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, 320-128, color));
	pos = RandomLevel::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20), 20, GAME_HEIGHT-128);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, 320-128, color));

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20/2), (GAME_HEIGHT-128)/2+16);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "homing"));
	}

	int tempRand = randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20/2)-(16+10), (GAME_HEIGHT-128)/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "bounce", "speed"))); //speed=tracking
	}
	tempRand = randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 40, 40);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "multishot", "blast")));
	}

	//traps:
	//PowerupManager::pushPowerup(new PowerSquare(80+32+40+20+16, 160, 3));
	//PowerupManager::pushPowerup(new PowerSquare(528-40-20-16, 160, 3));
	//PowerupManager::pushPowerup(new PowerSquare(320, 64+16, 3));
	//PowerupManager::pushPowerup(new PowerSquare(320, 320-64-16, 3));
	//traps aren't powerups because that's just stupid

	//PowerupManager::pushPowerup(new PowerSquare(320, 160, "banana"));
}

Level* FewObstaclesLevel::factory() {
	return new FewObstaclesLevel();
}

FewObstaclesLevel::FewObstaclesLevel() { return; }
