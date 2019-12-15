#pragma once
#include <unordered_map>
#include <vector>
#include "colorvalueholder.h"
#include "tank.h"
#include "wall.h"
#include "bullet.h"
#include "level.h"
#include "power.h"
#include "powersquare.h"

typedef void(*voidFunction)(void);
typedef Power*(*PowerFunction)(void);

//void nothingFunctionBase() { return; }
//void(*nothingFunction)() = nothingFunctionBase;

//double collisionConstant = 4; //used as an efficiency check(?)
//double drawMultiplier = 1;

extern const double PI;
extern ColorValueHolder backColor;

extern const double GAME_WIDTH;
extern const double GAME_HEIGHT;

extern const ColorValueHolder* defaultExplosionColor;

extern const Bullet* junkBullet;

extern const double POWER_WIDTH;
extern const double POWER_HEIGHT;
extern const double POWER_LINE_WIDTH;
extern const double POWER_OUTLINE_MULTIPLIER;

extern std::vector<Tank*> tanks;
extern std::vector<Wall*> walls;
extern std::vector<Bullet*> bullets;
extern std::vector<Level*> levels; //active levels
extern std::vector<PowerSquare*> powerups;


extern std::unordered_map<std::string, Level*> levelLookup;
extern std::unordered_map<std::string, PowerFunction> powerLookup;
