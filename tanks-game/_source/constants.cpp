#include "constants.h"

#define _USE_MATH_DEFINES
#include <cmath>

const double PI = M_PI;
const double GAME_WIDTH = 640;
const double GAME_HEIGHT = 320;
const double TANK_RADIUS = 16;
const double SHOOT_COOLDOWN = 100;
const double POWER_COOLDOWN = 500; //does nothing

const float DESTRUCTION_TIER = 2;
const double BULLET_TO_TANK_RADIUS_RATIO = .25;
const double BULLET_TO_TANK_SPEED_RATIO = 4; //2 isn't that bad

const float LOW_TIER = -999;
const float HIGH_TIER = 999;
const float LOW_IMPORTANCE = -1;
const float HIGH_IMPORTANCE = 999;

const Team_ID DEFAULT_TEAM = 0;
const Team_ID HAZARD_TEAM = -1;
const Game_ID NO_PARENT = -1;

//tank team rules:
//0  = no team or default team
//-1 = hazard team (hazards can be split up into multiple teams, but by default there's only one)
//1  = default teamID of tank #1 ("WASD")
//2  = default teamID of tank #2 ("Arrow Keys")
//tank teams always > 0
