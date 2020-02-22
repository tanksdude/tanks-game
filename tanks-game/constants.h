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
#include "res/vendor/glm/glm.hpp"
#include "circlehazard.h"
#include "recthazard.h"

//these 5 are all that's really needed:
extern const double PI; //this doesn't need to exist but typing the #define before the #include<math.h> is annoying
extern ColorValueHolder backColor;
extern const double GAME_WIDTH;
extern const double GAME_HEIGHT;
extern glm::mat4 proj;

//typedef void(*voidFunction)(void); //this isn't used

extern const ColorValueHolder* defaultExplosionColor;

extern const double POWER_WIDTH;
extern const double POWER_HEIGHT;
extern const double POWER_LINE_WIDTH;
extern const double POWER_OUTLINE_MULTIPLIER;

extern std::vector<Tank*> tanks;
extern std::vector<Wall*> walls;
//extern std::vector<Bullet*> bullets;
extern std::vector<Level*> levels; //active levels
extern std::vector<PowerSquare*> powerups;
extern std::vector<CircleHazard*> circleHazards;
extern std::vector<RectHazard*> rectHazards;

extern std::unordered_map<std::string, Level*> levelLookup;
extern std::vector<Level*> levelList;
extern std::vector<std::string> levelNameList;
typedef Power*(*PowerFunction)(void);
extern std::unordered_map<std::string, PowerFunction> powerLookup;
extern std::vector<PowerFunction> powerList;
extern std::vector<std::string> powerNameList;
