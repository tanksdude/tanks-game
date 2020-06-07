#include "corridorlevel.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "mylib.h"
#include "resetthings.h"

void CorridorLevel::initialize() {
	int randPos = randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), randPos);

	ColorValueHolder color = ColorValueHolder(0x22/255.0, 0.5, 1.0);

	for (int i = 0; i < 4; i++) {
		WallManager::pushWall(new Wall(320 - 240*(((3-i)/2) * 2 - 1) - 32*((((3-i)/2) + 1) % 2), i%2 * (320-128), 32, 128, color));
	}
	
	WallManager::pushWall(new Wall(320 - 90, 90, 20, 320 - 180, color));
	WallManager::pushWall(new Wall(320 + 70, 90, 20, 320 - 180, color));

	for (int i = 0; i < 4; i++)
		WallManager::pushWall(new Wall(112 + i % 2 * (320 + 18 - 112), 36 + (i / 2) * 230, 320 - 112 - 18, 18, color));

	PowerupManager::pushPowerup(new PowerSquare(320, 160, "bounce"));

	int tempRand = randFunc() * 2;
	for (int i = 0; i < 4; i++)
		PowerupManager::pushPowerup(new PowerSquare(320 + (i % 2 * 2 - 1) * 190, 160 + ((i / 2) * 2 - 1) * 142, RandomLevel::powerAlternate(i, tempRand, "invincible", "wallhack")));

	tempRand = randFunc() * 2;
	for (int i = 0; i < 4; i++)
		PowerupManager::pushPowerup(new PowerSquare(320 + (i % 2 * 2 - 1) * 190, 160 + ((i / 2) * 2 - 1) * 90, RandomLevel::powerAlternate(i, tempRand, "speed", "big"))); //big=life here
}

Level* CorridorLevel::factory() {
	return new CorridorLevel();
}

CorridorLevel::CorridorLevel() { return; }
