#pragma once

class CollisionObject {
	friend class CollisionHandler;
protected:
	double x;
	double y;

public:
	virtual void draw() = 0;
};