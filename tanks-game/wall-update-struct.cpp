#include "wall-update-struct.h"

WallUpdateStruct::WallUpdateStruct(double x, double y, double w, double h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void WallUpdateStruct::add(const WallUpdateStruct& other) {
	this->x += other.x;
	this->y += other.y;
	this->w += other.w;
	this->h += other.h;
}
