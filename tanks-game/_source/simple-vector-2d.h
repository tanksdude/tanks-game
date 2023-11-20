#pragma once
#include <iostream>

struct SimpleVector2D {
	friend class CollisionHandler;

protected:
	float xComp;
	float yComp;
	float angle;
	float magnitude; //nonnegative

private:
	SimpleVector2D(float xComp, float yComp, float angle, float magnitude); //raw constructor just for MakeVector_ComponentsAngle
public:
	SimpleVector2D(float xComp, float yComp);
	SimpleVector2D(float angle, float magnitude, bool angle_magnitude); //bool is just there to differentiate the constructors
	SimpleVector2D() : SimpleVector2D(0, 0) {}

	static SimpleVector2D MakeVector_Components(float xComp, float yComp);
	static SimpleVector2D MakeVector_Angle(float angle, float magnitude);
	static SimpleVector2D MakeVector_ComponentsMagnitude(float xComp, float yComp, float magnitude); //x and y to calculate the angle

	void setMagnitude(float magnitude);
	void setAngle(float angle);
	void changeMagnitude(float delta);
	void changeAngle(float delta);
	void multiplyMagnitude(float scale);
	void scaleAndRotate(float scale, float angle);

	float getXComp() const noexcept { return xComp; }
	float getYComp() const noexcept { return yComp; }
	float getAngle() const noexcept { return angle; }
	float getMagnitude() const noexcept { return magnitude; }

	SimpleVector2D operator+(const SimpleVector2D&);
	SimpleVector2D operator-(const SimpleVector2D&);
	friend std::ostream& operator<<(std::ostream& os, const SimpleVector2D&);

	static float dotProduct(const SimpleVector2D&, const SimpleVector2D&);
	static float crossProduct(const SimpleVector2D&, const SimpleVector2D&);
	static float angleBetween(const SimpleVector2D&, const SimpleVector2D&);
};
