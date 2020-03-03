#pragma once
#include "constants.h"
#include "res/vendor/glm/gtc/matrix_transform.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

const double PI = M_PI;
ColorValueHolder backColor = ColorValueHolder(0xDD/255.0, 0xDD/255.0, 0xDD/255.0);
const double GAME_WIDTH = 640;
const double GAME_HEIGHT = 320;
glm::mat4 proj = glm::ortho(0.0f, (float)GAME_WIDTH, 0.0f, (float)GAME_HEIGHT);
const double TANK_RADIUS = 16;

//std::vector<Tank*> tanks;
//std::vector<Wall*> walls;
//std::vector<Bullet*> bullets;
//std::vector<Level*> levels; //active levels
//std::vector<PowerSquare*> powerups;
//std::vector<CircleHazard*> circleHazards;
//std::vector<RectHazard*> rectHazards;

//TODO: figure out good solution to managing these
std::unordered_map<std::string, Level*> levelLookup;
std::vector<Level*> levelList;
std::vector<std::string> levelNameList;
std::unordered_map<std::string, PowerFunction> powerLookup;
std::vector<PowerFunction> powerList;
std::vector<std::string> powerNameList;

