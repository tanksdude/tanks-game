#include "constants.h"
#define _USE_MATH_DEFINES
#include <math.h>

const double PI = M_PI;
const double GAME_WIDTH = 640;
const double GAME_HEIGHT = 320;
const double TANK_RADIUS = 16;

const double DESTRUCTION_TIER = 2;
const double BULLET_TO_TANK_RADIUS_RATIO = .25;
const double BULLET_TO_TANK_SPEED_RATIO = 4; //2 isn't that bad

const char DEFAULT_TEAM = 0;
const char HAZARD_TEAM = -1;
