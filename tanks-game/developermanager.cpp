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
std::vector<std::string> DeveloperManager::insertListIdentifiers = { "longinvincible", "banana", "mines", "godmode", "inversion", "stationary turret" };

double DeveloperManager::getX(Circle* c) { return c->x; }
double DeveloperManager::getY(Circle* c) { return c->y; }
double DeveloperManager::getR(Circle* c) { return c->r; }

double DeveloperManager::getX(Rect* r) { return r->x; }
double DeveloperManager::getY(Rect* r) { return r->y; }
double DeveloperManager::getW(Rect* r) { return r->w; }
double DeveloperManager::getH(Rect* r) { return r->h; }

void DeveloperManager::mouseDragFunc(int x, int y) {
	//dev tools
	int real_x = x;
	int real_y = y - (Renderer::window_height - Renderer::gamewindow_height);
	if (leftMouse) {
		if (!rightMouse) { //tank 1
			TankManager::getTank(0)->x = (real_x / double(Renderer::gamewindow_width)) * GAME_WIDTH;
			TankManager::getTank(0)->y = (1 - real_y / double(Renderer::gamewindow_height)) * GAME_HEIGHT;
		}
		else { //tank 2
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
			insertIndex = ((insertIndex % insertIndexMax) + 1) % insertIndexMax;
		} else {
			insertIndex = 0;
		}
	}
	else { //scroll down
		if (insertIndexMax > 0) {
			insertIndex = ((insertIndex % insertIndexMax) - 1 + insertIndexMax) % insertIndexMax;
		} else {
			insertIndex = 0;
		}
	}
	//accounting for potential mistakes is annoying
	//std::cout << "wheel:" << wheel << ", dir:" << dir << std::endl;

	std::cout << "DeveloperManager insertIdentifier: " << insertListIdentifiers[insertIndex] << std::endl;
}

void DeveloperManager::devInsert(int x, int y) {
	switch (insertIndex) {
		case 0:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "longinvincible"));
			return;
		case 1:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "banana"));
			return;
		case 2:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla-extra", "mines"));
			return;
		case 3:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "godmode"));
			return;
		case 4:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "inversion"));
			return;
		case 5:
			std::string paras[3] = { std::to_string(x), std::to_string(y), std::to_string(0) };
			HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary turret")(3, paras));
			return;
	}
	//no default
}
