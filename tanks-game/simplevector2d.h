#pragma once
#include <iostream>

struct SimpleVector2D {
	friend class CollisionHandler;
protected:
	float xComp;
	float yComp;
	float angle;
	float magnitude; //nonnegative

public:
	SimpleVector2D(float xComp, float yComp);
	SimpleVector2D(float angle, float magnitude, bool angle_magnitude); //bool is just there to differentiate the constructors
	SimpleVector2D() : SimpleVector2D(0, 0) {}
	SimpleVector2D(const SimpleVector2D&);

	void setMagnitude(float magnitude);
	void setAngle(float angle);
	void changeMagnitude(float delta);
	void changeAngle(float delta);

	float getXComp() const { return xComp; }
	float getYComp() const { return yComp; }
	float getAngle() const { return angle; }
	float getMagnitude() const { return magnitude; }

	SimpleVector2D operator+(const SimpleVector2D&);
	SimpleVector2D operator-(const SimpleVector2D&);
	friend std::ostream& operator<<(std::ostream& os, const SimpleVector2D&);

	static float dotProduct(const SimpleVector2D&, const SimpleVector2D&);
	static float crossProduct(const SimpleVector2D&, const SimpleVector2D&);
	static float angleBetween(const SimpleVector2D&, const SimpleVector2D&);
};
