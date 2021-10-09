#include "developermanager.h"
#include "tankmanager.h"
#include "renderer.h"
#include "constants.h"
#include "powerupmanager.h"
#include "hazardmanager.h"
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
	}
	else { //scroll down
		if (insertIndexMax > 0) {
			insertIndex = ((insertIndex % insertIndexMax) - 1 + insertIndexMax) % insertIndexMax; //accounting for potential mistakes/trolls
		} else {
			insertIndex = 0;
		}
	}
	//std::cout << "wheel:" << wheel << ", dir:" << dir << std::endl;

	std::cout << "DeveloperManager insertIdentifier: " << insertListIdentifiers[insertIndex] << std::endl;
}

std::vector<std::string> DeveloperManager::insertListIdentifiers = { "longinvincible", "temp", "banana", "barrier", "mines", "godmode", "inversion", "stationary_turret" };
void DeveloperManager::devInsert(int x, int y) {
	switch (insertIndex) {
		case 0:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "longinvincible"));
			break; //return is okay because the power just needs to be added, nothing more, but I don't want to forget that you use break not return
		case 1:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla-extra", "shield"));
			break;
		case 2:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "banana"));
			break;
		case 3:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla-extra", "barrier"));
			break;
		case 4:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla-extra", "mines"));
			break;
		case 5:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "godmode"));
			break;
		case 6:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "inversion"));
			break;
		case 7:
			std::string paras[3] = { std::to_string(x), std::to_string(y), std::to_string(0) };
			HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(3, paras));
			break;
	}
	//no default
}
