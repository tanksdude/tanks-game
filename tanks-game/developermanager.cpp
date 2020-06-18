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
			PowerupManager::pushPowerup(new PowerSquare(true_x, true_y, "dev", "longinvincible")); //change to godmode when it exists?
		}
	} else {
		if (button == GLUT_LEFT_BUTTON) {
			leftMouse = false;
		}
	}
}

void DeveloperManager::mouseWheelFunc(int wheel, int dir, int x, int y) {
	// in the future, the wheel should change the index of some list of stuffs as a dev menu to insert said stuffs (and middle mouse inserts the thing)
	int real_x = (x / double(Renderer::window_width)) * GAME_WIDTH;
	int real_y = (1 - y / double(Renderer::window_height)) * GAME_HEIGHT;

	if (dir == 1) { //scroll up
		std::string powers[2] = { "homing", "bounce" };
		PowerupManager::pushPowerup(new PowerSquare(real_x, real_y, powers, 2));
	}
	else { //scroll down
		std::string powers[1] = { "banana" };
		PowerupManager::pushPowerup(new PowerSquare(real_x, real_y, powers, 1));
	}

	//std::cout << "wheel:" << wheel << ", dir:" << dir << std::endl;
}
