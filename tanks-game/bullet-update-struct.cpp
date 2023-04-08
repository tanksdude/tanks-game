#include "bullet-update-struct.h"

BulletUpdateStruct::BulletUpdateStruct(double x, double y, double r, double speed, double angle, double alpha) {
	//add acceleration?
	this->x = x;
	this->y = y;
	this->r = r;
	this->speed = speed;
	this->angle = angle;
	this->alpha = alpha;
}

void BulletUpdateStruct::add(const BulletUpdateStruct& other) {
	this->x += other.x;
	this->y += other.y;
	this->r += other.r;
	this->speed += other.speed;
	this->angle += other.angle;
	this->alpha += other.alpha;
}
