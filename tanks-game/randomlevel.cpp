#pragma once
#include "randomlevel.h"
#include "constants.h"
#include "tank.h"
#include "powersquare.h"
#include <time.h>
#include <math.h>

void RandomLevel::initialize() {
	int randPos = rand() % 5;
	tanks[0]->y = randPos*64 + 32;
	tanks[1]->y = (4-randPos)*64 + 32;
	//reset power and shooting

	ColorValueHolder randColor(rand()%256, rand()%256, rand()%256);

	for (int i = 0; i < 16; i++) {
		walls.push_back(RandomLevel::makeNewWall(40, 32, GAME_WIDTH - 2*40, GAME_HEIGHT - 2*32, randColor));
	}

	powerups.push_back(new PowerSquare(20, 20, "speed"));
	powerups.push_back(new PowerSquare(40, 20, "wallhack"));
	//powerups.push_back(new PowerSquare(60, 20, "bounce"));
	powerups.push_back(new PowerSquare(80, 20, "multishot"));
}

Wall* RandomLevel::makeNewWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, ColorValueHolder c, double minW, double minH, double maxW, double maxH) {

	double w = fmod((double)rand(), (maxW - minW)) + minW;
	double h = fmod((double)rand(), (maxH - minH)) + minH;

	return new Wall(x_beginning + fmod((double)rand(), (width_ofArea - w)), y_beginning + fmod((double)rand(), (height_ofArea - h)), w, h, c);
}

RandomLevel::RandomLevel() { return; }
