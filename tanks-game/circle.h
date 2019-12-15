#pragma once
#include "collisionobject.h"

class Circle : public CollisionObject {
	friend class CollisionHandler;
public:
	double x;
	double y;
	double r;
	
public:
	static const unsigned short numOfSides = 64; //need a short?

};