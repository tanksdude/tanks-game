#pragma once
#include "randomlevel.h"
#include "constants.h"
#include "tank.h"
#include "powersquare.h"
#include "mylib.h"
#include <time.h>
#include <math.h>
#include "stationaryturret.h" //TODO: vector and hashtable of hazards
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"

void RandomLevel::initialize() {
	int randPos = rand() % 5;
	tanks[0]->y = randPos*64 + 32;
	tanks[1]->y = (4-randPos)*64 + 32;
	//reset power and shooting

	ColorValueHolder randColor((rand()%256)/255.0, (rand()%256)/255.0, (rand()%256)/255.0);

	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(RandomLevel::makeNewWall(40, 32, GAME_WIDTH - 2*40, GAME_HEIGHT - 2*32, randColor));
	}

	HazardManager::pushCircleHazard(new StationaryTurret(GAME_WIDTH/2, GAME_HEIGHT/2, double(rand())/double(RAND_MAX+1) * 2*PI));

	PowerupManager::pushPowerup(new PowerSquare(20, 20, "speed"));
	PowerupManager::pushPowerup(new PowerSquare(40, 20, "wallhack"));
	PowerupManager::pushPowerup(new PowerSquare(60, 20, "bounce"));
	PowerupManager::pushPowerup(new PowerSquare(80, 20, "multishot"));
	PowerupManager::pushPowerup(new PowerSquare(100, 20, "triple"));
	PowerupManager::pushPowerup(new PowerSquare(120, 20, "homing"));
	PowerupManager::pushPowerup(new PowerSquare(140, 20, "invincible"));
	PowerupManager::pushPowerup(new PowerSquare(160, 20, "grenade"));
	PowerupManager::pushPowerup(new PowerSquare(180, 20, "fire"));
	PowerupManager::pushPowerup(new PowerSquare(200, 20, "blast"));

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2 + 20, "big"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2 - 20, "megadeath"));

	std::string* names = new std::string[2]{ "multishot", "multishot" };
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-20, GAME_HEIGHT-20, names, 2));
	names[0] = "speed", names[1] = "wallhack";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-40, GAME_HEIGHT-20, names, 2));
	names[0] = "bounce", names[1] = "homing";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-60, GAME_HEIGHT-20, names, 2));
	names[0] = "bounce", names[1] = "big";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-80, GAME_HEIGHT-20, names, 2));
	names[0] = "wallhack", names[1] = "grenade";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-100, GAME_HEIGHT-20, names, 2));
	delete[] names;
}

Wall* RandomLevel::makeNewWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, ColorValueHolder c, double minW, double minH, double maxW, double maxH) {
	//fix: not sure if rand() needs to be cast to a double, but worry about that later
	double w = fmod((double)rand(), (maxW - minW)) + minW;
	double h = fmod((double)rand(), (maxH - minH)) + minH;

	return new Wall(x_beginning + fmod((double)rand(), (width_ofArea - w)), y_beginning + fmod((double)rand(), (height_ofArea - h)), w, h, c);
}

std::string* RandomLevel::getRandomPowers(int count, bool replacement, int nameCount, std::string* names) {
	std::string* n = new std::string[count];
	int* used = new int[count];

	for (int i = 0; i < count; i++) {
		if (!replacement) {
			//first verify if a new name can be chosen:
			bool found = true;
			for (int j = 0; j < i; j++) {
				if (!isInArray<int>(j, used, i+1)) {
					found = false;
					break;
				}
			}
			//then add name based on result
			if (!found) {
				//there are values that can get chosen
				int index;
				do {
					index = rand() % nameCount;
				} while (isInArray<int>(index, used, i+1));
				used[i] = index;
			} else {
				//there aren't values that can get chosen
				replacement = true; //instead of getting stuck or throwing an error, just put duplicates in (change eventually)
			}
		}
		if(replacement) {
			int index = rand() % nameCount;
			used[i] = index;
		}
	}

	for (int i = 0; i < count; i++) {
		n[i] = names[used[i]];
	}

	delete[] used;
	return n;
}

RandomLevel::RandomLevel() { return; }
