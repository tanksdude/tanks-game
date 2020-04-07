#pragma once
#include "developermanager.h"

double DeveloperManager::getX(Circle* c) { return c->x; }
double DeveloperManager::getY(Circle* c) { return c->y; }
double DeveloperManager::getR(Circle* c) { return c->r; }

double DeveloperManager::getX(Rect* r) { return r->x; }
double DeveloperManager::getY(Rect* r) { return r->y; }
double DeveloperManager::getW(Rect* r) { return r->w; }
double DeveloperManager::getH(Rect* r) { return r->h; }

