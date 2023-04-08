#pragma once

class WallUpdateStruct {
public:
	//deltas:
	double x;
	double y;
	double w;
	double h;
	//absolutes:
	//ColorValueHolder color;

	void add(const WallUpdateStruct& other);

	WallUpdateStruct(double x, double y, double w, double h);
	WallUpdateStruct() : WallUpdateStruct(0, 0, 0, 0) {}
};
