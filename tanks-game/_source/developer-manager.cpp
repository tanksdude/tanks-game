#include "developer-manager.h"

#include "constants.h"
#include <iostream>

#include <GL/glew.h> //to avoid potential glfw before glew issues
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "level-manager.h"

bool DeveloperManager::leftMouse = false;
bool DeveloperManager::rightMouse = false;
bool DeveloperManager::middleMouse = false;
int DeveloperManager::insertIndex = 0;

int DeveloperManager::mousePosX = 0;
int DeveloperManager::mousePosY = 0;

void DeveloperManager::setX(Circle* c, double x) { c->x = x; }
void DeveloperManager::setY(Circle* c, double y) { c->y = y; }
void DeveloperManager::setR(Circle* c, double r) { c->r = r; }

void DeveloperManager::setX(Rect* r, double x) { r->x = x; }
void DeveloperManager::setY(Rect* r, double y) { r->y = y; }
void DeveloperManager::setW(Rect* r, double w) { r->w = w; }
void DeveloperManager::setH(Rect* r, double h) { r->h = h; }

void DeveloperManager::mouseButtonCallbackFunc(GLFWwindow* window, int button, int action, int mods) {
	switch (button) {
		case GLFW_MOUSE_BUTTON_1:
			leftMouse = (action == GLFW_PRESS);
			break;
		case GLFW_MOUSE_BUTTON_2:
			if (action == GLFW_PRESS) {
				rightMouse = !rightMouse; //bad name but whatever
			}
			break;
		case GLFW_MOUSE_BUTTON_3:
			middleMouse = (action == GLFW_PRESS);
			break;
	}

	if (middleMouse) {
		std::cout << "middle mouse" << std::endl;
		devInsert(mousePosX, mousePosY);
		middleMouse = false;
	}
}

void DeveloperManager::mouseScrollCallbackFunc(GLFWwindow* window, double xOffset, double yOffset) {
	int scrollAmount;

	if (yOffset == 0) {
		//left/right scrolling happened
		return;
	}
	if (yOffset > 0) {
		scrollAmount = std::max(static_cast<int>(yOffset), 1);
	} else {
		scrollAmount = std::min(static_cast<int>(yOffset), -1);
	}

	int insertIndexMax = insertListIdentifiers.size();
	if (insertIndexMax > 0) {
		insertIndex = ((insertIndex + scrollAmount) % insertIndexMax + insertIndexMax) % insertIndexMax;
	} else { [[unlikely]]
		insertIndex = 0;
	}

	std::cout << "DeveloperManager insert identifier: " << insertListIdentifiers[insertIndex] << std::endl;
}

void DeveloperManager::mouseCursorPosCallbackFunc(GLFWwindow* window, double xPos, double yPos) {
	//dev tools
	int real_x = xPos;
	int real_y = yPos - (Renderer::window_height - Renderer::gamewindow_height);

	mousePosX = static_cast<int>((real_x / double(Renderer::gamewindow_width)) * GAME_WIDTH);
	mousePosY = static_cast<int>((1 - real_y / double(Renderer::gamewindow_height)) * GAME_HEIGHT);

	if (leftMouse) {
		if (!rightMouse) { //tank 1
			TankManager::getTank(0)->x = mousePosX;
			TankManager::getTank(0)->y = mousePosY;
		} else { //tank 2
			TankManager::getTank(1)->x = mousePosX;
			TankManager::getTank(1)->y = mousePosY;
		}
	}
}

std::vector<std::string> DeveloperManager::insertListIdentifiers = { "longinvincible", "temp", "banana", "homing", "barrier", "bounce", "mines", "multishot", "grenade", "godmode", "big", "inversion", "annoying", "stationary_turret", "vert_wall", "horz_wall" };
void DeveloperManager::devInsert(int x, int y) {
	GenericFactoryConstructionData constructionData;
	double* posArr;
	switch (insertIndex) {
		case 0:
			#if _DEBUG
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "longinvincible"));
			#else
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "longinvincible"));
			#endif
			break;
		case 1:
			//PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla", "blast"));
			//PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "ultrabounce"));
			//PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "other_stuff_is_poison"));
			//PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "backwards_movement"));
			//PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "wall_sparks"));
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "weird_extra_cannons"));
			break;
		case 2:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "banana"));
			break;
		case 3:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "homing"));
			break;
		case 4:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla-extra", "barrier"));
			break;
		case 5:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla", "bounce"));
			break;
		case 6:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla-extra", "mines"));
			break;
		case 7:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "multishot"));
			break;
		case 8:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "grenade"));
			break;
		case 9:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "godmode"));
			break;
		case 10:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "big"));
			break;
		case 11:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "inversion"));
			break;
		case 12:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "annoying"));
			break;
		case 13:
			posArr = new double[3]{ double(x), double(y), 0 };
			constructionData = GenericFactoryConstructionData(3, posArr);
			HazardManager::pushCircleHazard("vanilla", "stationary_turret", constructionData);
			break;
		case 14:
			WallManager::pushWall(new Wall(x, y, 20, 60, LevelManager::getLevel(0)->getDefaultColor()));
			break;
		case 15:
			WallManager::pushWall(new Wall(x, y, 60, 20, LevelManager::getLevel(0)->getDefaultColor()));
			break;
		default:
			//better than nothing happening
			PowerupManager::pushPowerup(new PowerSquare(x, y, "speed"));
			break;
	}
}
