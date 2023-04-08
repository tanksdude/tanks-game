#pragma once

class BulletUpdateStruct {
public:
	//deltas:
	double x;
	double y;
	double r;
	double speed;
	double angle;
	double alpha;
	//absolutes:
	//double acceleration;

	void add(const BulletUpdateStruct& other);

	BulletUpdateStruct(double x, double y, double r, double speed, double angle, double alpha);
	BulletUpdateStruct() : BulletUpdateStruct(0, 0, 0, 0, 0, 0) {}
};
