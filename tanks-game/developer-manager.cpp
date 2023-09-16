#include "developer-manager.h"
#include "tank-manager.h"
#include "renderer.h"
#include "constants.h"
#include "wall-manager.h"
#include "powerup-manager.h"
#include "hazard-manager.h"
#include "level-manager.h"
#include <GL/glew.h> //to avoid potential freeglut before glew issues
#include <GL/freeglut.h>
#include <iostream>

bool DeveloperManager::leftMouse = false;
bool DeveloperManager::rightMouse = false;
int DeveloperManager::insertIndex = 0;

void DeveloperManager::setX(Circle* c, double x) { c->x = x; }
void DeveloperManager::setY(Circle* c, double y) { c->y = y; }
void DeveloperManager::setR(Circle* c, double r) { c->r = r; }

void DeveloperManager::setX(Rect* r, double x) { r->x = x; }
void DeveloperManager::setY(Rect* r, double y) { r->y = y; }
void DeveloperManager::setW(Rect* r, double w) { r->w = w; }
void DeveloperManager::setH(Rect* r, double h) { r->h = h; }

void DeveloperManager::mouseDragFunc(int x, int y) {
	//dev tools
	int real_x = x;
	int real_y = y - (Renderer::window_height - Renderer::gamewindow_height);
	if (leftMouse) {
		if (!rightMouse) { //tank 1
			TankManager::getTank(0)->x = (real_x / double(Renderer::gamewindow_width)) * GAME_WIDTH;
			TankManager::getTank(0)->y = (1 - real_y / double(Renderer::gamewindow_height)) * GAME_HEIGHT;
		} else { //tank 2
			TankManager::getTank(1)->x = (real_x / double(Renderer::gamewindow_width)) * GAME_WIDTH;
			TankManager::getTank(1)->y = (1 - real_y / double(Renderer::gamewindow_height)) * GAME_HEIGHT;
		}
	}
}

void DeveloperManager::mouseClickFunc(int button, int state, int x, int y) {
	int real_x = x;
	int real_y = y - (Renderer::window_height - Renderer::gamewindow_height);
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			leftMouse = true;
		} else if (button == GLUT_RIGHT_BUTTON) {
			rightMouse = !rightMouse;
		} else { //button == GLUT_MIDDLE_BUTTON
			double true_x = (real_x / double(Renderer::gamewindow_width)) * GAME_WIDTH;
			double true_y = (1 - real_y / double(Renderer::gamewindow_height)) * GAME_HEIGHT;
			devInsert(true_x, true_y);
		}
	} else {
		if (button == GLUT_LEFT_BUTTON) {
			leftMouse = false;
		}
	}
}

void DeveloperManager::mouseWheelFunc(int wheel, int dir, int x, int y) {
	int real_x = (x / double(Renderer::window_width)) * GAME_WIDTH;
	int real_y = (1 - y / double(Renderer::window_height)) * GAME_HEIGHT;

	int insertIndexMax = insertListIdentifiers.size();
	if (dir == 1) { //scroll up
		if (insertIndexMax > 0) {
			insertIndex = ((insertIndex % insertIndexMax) + 1 + insertIndexMax) % insertIndexMax; //accounting for potential mistakes/trolls
		} else {
			insertIndex = 0;
		}
	} else { //scroll down
		if (insertIndexMax > 0) {
			insertIndex = ((insertIndex % insertIndexMax) - 1 + insertIndexMax) % insertIndexMax; //accounting for potential mistakes/trolls
		} else {
			insertIndex = 0;
		}
	}
	//std::cout << "wheel:" << wheel << ", dir:" << dir << std::endl;

	std::cout << "DeveloperManager insertIdentifier: " << insertListIdentifiers[insertIndex] << std::endl;
}

std::vector<std::string> DeveloperManager::insertListIdentifiers = { "longinvincible", "temp", "banana", "homing", "barrier", "bounce", "mines", "multishot", "grenade", "godmode", "big", "inversion", "annoying", "stationary_turret", "vert_wall", "horz_wall" };
void DeveloperManager::devInsert(int x, int y) {
	GenericFactoryConstructionData constructionData;
	double* posArr;
	switch (insertIndex) {
		case 0:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "longinvincible"));
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
			HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(constructionData));
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
