#pragma once
#include "collisionobject.h"

class Rect : public CollisionObject{
	friend class CollisionHandler;
protected:
	double w;
	double h;
};