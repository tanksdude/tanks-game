#pragma once
#include "developermanager.h"
#include "tankmanager.h"
#include "renderer.h"
#include "constants.h"
#include "powerupmanager.h"
#include "hazardmanager.h"
#include <string>
#include <GL/glut.h>
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
	if (leftMouse) {
		if (!rightMouse) { //tank 1
			TankManager::getTank(0)->x = (x / double(Renderer::window_width)) * GAME_WIDTH;
			TankManager::getTank(0)->y = (1 - y / double(Renderer::window_height)) * GAME_HEIGHT;
		}
		else { //tank 2
			TankManager::getTank(1)->x = (x / double(Renderer::window_width)) * GAME_WIDTH;
			TankManager::getTank(1)->y = (1 - y / double(Renderer::window_height)) * GAME_HEIGHT;
		}
	}
	//positions are off when window aspect ratio isn't 2:1
}

void DeveloperManager::mouseClickFunc(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			leftMouse = true;
		} else if (button == GLUT_RIGHT_BUTTON) {
			rightMouse = !rightMouse;
		}
	} else {
		if (button == GLUT_LEFT_BUTTON) {
			leftMouse = false;
		}
	}
}

void DeveloperManager::mouseWheelFunc(int wheel, int dir, int x, int y) {
	// in the future, the wheel should change the index of some list of stuffs as a dev menu to insert said stuffs
	int real_x = (x / double(Renderer::window_width)) * GAME_WIDTH;
	int real_y = (1 - y / double(Renderer::window_height)) * GAME_HEIGHT;

	if (dir == 1) { //scroll up
		std::string* powers = new std::string[2]{ "homing", "bounce" };
		PowerupManager::pushPowerup(new PowerSquare(real_x, real_y, powers, 2));
		delete[] powers;
	}
	else { //scroll down
		std::string* paras = new std::string[3]{ std::to_string(real_x), std::to_string(real_y), std::to_string(0) };
		HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("stationary turret")(3, paras));
		delete[] paras;
	}

	//std::cout << "wheel:" << wheel << ", dir:" << dir << std::endl;
}