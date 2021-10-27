#pragma once

typedef long Game_ID;
typedef char Team_ID;

extern const double PI; //this doesn't need to exist but typing the #define before the #include<math.h> is annoying
extern const double GAME_WIDTH;
extern const double GAME_HEIGHT;
extern const double TANK_RADIUS;

extern const double DESTRUCTION_TIER; //offense tier for destorying walls and turrets & stuff
extern const double BULLET_TO_TANK_RADIUS_RATIO;
extern const double BULLET_TO_TANK_SPEED_RATIO;

extern const Team_ID DEFAULT_TEAM; //walls and friends are this team
extern const Team_ID HAZARD_TEAM;  //default team of hazards

//typedef void(*voidFunction)(void); //this isn't used
//using voidFunction = void(*)(void); //C++11 version
