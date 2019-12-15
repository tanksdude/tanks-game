#pragma once
#include "constants.h"

typedef void(*voidFunction)(void);

//double collisionConstant = 4; //used as an efficiency check(?)
//double drawMultiplier = 1;

const double PI = 3.141592653589793238462643383279502884L;
extern ColorValueHolder backColor = ColorValueHolder(0xDD, 0xDD, 0xDD);

const double GAME_WIDTH = 640;
const double GAME_HEIGHT = 320;

ColorValueHolder* defaultExplosionColor = new ColorValueHolder(0, 0, 0);

const double HIGHEST_REASONABLE_TIER = 9999;
const Bullet* junkBullet = new Bullet(0, 0, 0, 0, 0, -1);

const double POWER_WIDTH = 4;
const double POWER_HEIGHT = 4;
const double POWER_OUTLINE_MULTIPLIER = 1.5;

std::vector<Tank*> tanks;
std::vector<Wall*> walls;
std::vector<Bullet*> bullets;
std::vector<Level*> levels; //active levels

std::vector<WallEdge*> edges;


std::unordered_map<std::string, Level*> levelLookup;
//std::unordered_map<std::string, Power*> powerLookup;
