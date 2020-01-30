#pragma once
#include "constants.h"
#include "res/vendor/glm/gtc/matrix_transform.hpp"

const double PI = 3.141592653589793238462643383279502884L;
ColorValueHolder backColor = ColorValueHolder(0xDD/255.0, 0xDD/255.0, 0xDD/255.0);

const double GAME_WIDTH = 640;
const double GAME_HEIGHT = 320;
glm::mat4 proj = glm::ortho(0.0f, (float)GAME_WIDTH, 0.0f, (float)GAME_HEIGHT);

const ColorValueHolder* defaultExplosionColor = new ColorValueHolder(0, 0, 0);

const double POWER_WIDTH = 6;
const double POWER_HEIGHT = 6;
const double POWER_LINE_WIDTH = .333;
const double POWER_OUTLINE_MULTIPLIER = 1.5;

std::vector<Tank*> tanks;
std::vector<Wall*> walls;
std::vector<Bullet*> bullets;
std::vector<Level*> levels; //active levels
std::vector<PowerSquare*> powerups;

std::unordered_map<std::string, Level*> levelLookup;
std::vector<Level*> levelList;
std::unordered_map<std::string, PowerFunction> powerLookup;
std::vector<PowerFunction> powerList;

