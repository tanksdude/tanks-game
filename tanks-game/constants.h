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

typedef void(*voidFunction)(void);
typedef Power*(*PowerFunction)(void);

extern const double PI; //so this exists in math.h ... I'll remove it later
extern ColorValueHolder backColor;

extern const double GAME_WIDTH;
extern const double GAME_HEIGHT;
extern glm::mat4 proj;

extern const ColorValueHolder* defaultExplosionColor;

extern const double POWER_WIDTH;
extern const double POWER_HEIGHT;
extern const double POWER_LINE_WIDTH;
extern const double POWER_OUTLINE_MULTIPLIER;

extern std::vector<Tank*> tanks;
extern std::vector<Wall*> walls;
extern std::vector<Bullet*> bullets;
extern std::vector<Level*> levels; //active levels
extern std::vector<PowerSquare*> powerups;
extern std::vector<CircleHazard*> circleHazards;
extern std::vector<RectHazard*> rectHazards;

extern std::unordered_map<std::string, Level*> levelLookup;
extern std::vector<Level*> levelList;
extern std::vector<std::string> levelNameList;
extern std::unordered_map<std::string, PowerFunction> powerLookup;
extern std::vector<PowerFunction> powerList;
extern std::vector<std::string> powerNameList;
