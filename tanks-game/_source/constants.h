#pragma once

typedef int Game_ID;
typedef char Team_ID;

extern const double PI;
extern const double GAME_WIDTH;
extern const double GAME_HEIGHT;
extern const double TANK_RADIUS;
extern const double SHOOT_COOLDOWN;
extern const double POWER_COOLDOWN;

extern const double BULLET_TO_TANK_RADIUS_RATIO;
extern const double BULLET_TO_TANK_SPEED_RATIO;

extern const float DESTRUCTION_TIER; //offense tier for destorying walls and turrets & stuff
extern const float LOW_TIER;
extern const float HIGH_TIER;
extern const float LOW_IMPORTANCE;
extern const float HIGH_IMPORTANCE;

extern const Team_ID DEFAULT_TEAM; //walls and friends are this team
extern const Team_ID HAZARD_TEAM;  //default team of hazards
extern const Game_ID NO_PARENT;    //bullets that don't have a parent for parentID

//typedef void(*voidFunction)(void); //this isn't used
//using voidFunction = void(*)(void); //C++11 version
