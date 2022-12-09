#include "constants.h"
#define _USE_MATH_DEFINES
#include <cmath>

const double PI = M_PI;
const double GAME_WIDTH = 640;
const double GAME_HEIGHT = 320;
const double TANK_RADIUS = 16;

const double DESTRUCTION_TIER = 2;
const double BULLET_TO_TANK_RADIUS_RATIO = .25;
const double BULLET_TO_TANK_SPEED_RATIO = 4; //2 isn't that bad

const double LOW_TIER = -999;
const double HIGH_TIER = 999;
const double LOW_IMPORTANCE = -1;
const double HIGH_IMPORTANCE = 999;

const Team_ID DEFAULT_TEAM = 0;
const Team_ID HAZARD_TEAM = -1;
const Game_ID NO_PARENT = -1;
