#pragma once
#include "randomlevel.h"
#include "constants.h"
#include "tank.h"
#include <time.h>
#include <math.h>

void RandomLevel::initialize() {
	int randPos = rand() % 5;
	tanks[0]->y = randPos*64 + 32;
	tanks[1]->y = (4-randPos)*64 + 32;
	//reset power and shooting

	ColorValueHolder randColor(rand()%256, rand()%256, rand()%256);

	for (int i = 0; i < 16; i++) {
		walls.push_back(RandomLevel::makeNewWall(32, 32, GAME_WIDTH - 2*32, GAME_HEIGHT - 2*32, randColor));
	}
}

Wall* RandomLevel::makeNewWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, ColorValueHolder c, double minW, double minH, double maxW, double maxH) {

	double w = fmod((double)rand(), (maxW - minW)) + minW;
	double h = fmod((double)rand(), (maxH - minH)) + minH;

	return new Wall(x_beginning + fmod((double)rand(), (width_ofArea - w)), y_beginning + fmod((double)rand(), (height_ofArea - h)), w, h, c);
}

RandomLevel::RandomLevel() { return; }
