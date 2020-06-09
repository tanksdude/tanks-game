#include "fewobstacleslevel.h"
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

	WallManager::pushWall(new Wall(80+32+40, 64, 20, 320-128, color));
	WallManager::pushWall(new Wall(528-40-20, 64, 20, 320-128, color));

	PowerupManager::pushPowerup(new PowerSquare(80+32+40+10, 64-16, "homing"));
	PowerupManager::pushPowerup(new PowerSquare(80+32+40+10, 64+320-128+16, "homing"));
	PowerupManager::pushPowerup(new PowerSquare(528-40-20+10, 64-16, "homing"));
	PowerupManager::pushPowerup(new PowerSquare(528-40-20+10, 64+320-128+16, "homing"));

	int tempRand = randFunc() * 2;
	for(int i = 0; i < 4; i++)
		PowerupManager::pushPowerup(new PowerSquare(320-(i%2*2-1)*132, 160-((i/2)*2-1)*76, RandomLevel::powerAlternate(i, tempRand, "bounce", "speed"))); //speed=tracking
	tempRand = randFunc() * 2;
	for(int i = 0; i < 4; i++)
		PowerupManager::pushPowerup(new PowerSquare(320-(i%2*2-1)*40, 160-((i/2)*2-1)*40, RandomLevel::powerAlternate(i, tempRand, "multishot", "blast")));

	//traps:
	//PowerupManager::pushPowerup(new PowerSquare(80+32+40+20+16, 160, 3));
	//PowerupManager::pushPowerup(new PowerSquare(528-40-20-16, 160, 3));
	//PowerupManager::pushPowerup(new PowerSquare(320, 64+16, 3));
	//PowerupManager::pushPowerup(new PowerSquare(320, 320-64-16, 3));

	//PowerupManager::pushPowerup(new PowerSquare(320, 160, "banana"));
}

Level* FewObstaclesLevel::factory() {
	return new FewObstaclesLevel();
}

FewObstaclesLevel::FewObstaclesLevel() { return; }
