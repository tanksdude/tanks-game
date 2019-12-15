#pragma once

class CollisionObject {
	friend class CollisionHandler;
protected:
	double x;
	double y;

public:
	virtual void draw() = 0; //fix: needed? it's only so a CollisionObject object can't be created
};