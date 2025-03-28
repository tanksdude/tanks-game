#include "simple-vector-2d.h"

#include "constants.h"
#include <cmath>
#include <algorithm> //std::clamp

SimpleVector2D::SimpleVector2D(float xComp, float yComp) {
	this->xComp = xComp;
	this->yComp = yComp;
	this->angle = std::atan2(yComp, xComp);
	this->magnitude = std::sqrt(xComp*xComp + yComp*yComp);
	//hypot() is slow, don't use it (unless you're dealing with massive values that could overflow)
	//https://stackoverflow.com/questions/32435796/when-to-use-stdhypotx-y-over-stdsqrtxx-yy
}

SimpleVector2D::SimpleVector2D(float angle, float magnitude, bool angle_magnitude) {
	this->angle = angle;
	this->magnitude = magnitude;
	this->xComp = magnitude * std::cos(angle);
	this->yComp = magnitude * std::sin(angle);
}

SimpleVector2D::SimpleVector2D(float xComp, float yComp, float angle, float magnitude) {
	this->xComp = xComp;
	this->yComp = yComp;
	this->angle = angle;
	this->magnitude = magnitude;
}

SimpleVector2D SimpleVector2D::MakeVector_Components(float xComp, float yComp) {
	return { xComp, yComp };
}

SimpleVector2D SimpleVector2D::MakeVector_Angle(float angle, float magnitude) {
	return { angle, magnitude, true };
}

SimpleVector2D SimpleVector2D::MakeVector_ComponentsMagnitude(float xComp, float yComp, float magnitude) {
	float angle = std::atan2(yComp, xComp);
	float newXComp = magnitude * std::cos(angle);
	float newYComp = magnitude * std::sin(angle);
	return { newXComp, newYComp, angle, magnitude };
}

void SimpleVector2D::setMagnitude(float mag) {
	if (mag <= 0) {
		this->magnitude = 0;
		this->xComp = 0;
		this->yComp = 0;
	} else {
		this->magnitude = mag;
		this->xComp = mag * std::cos(this->angle);
		this->yComp = mag * std::sin(this->angle);
	}
}

void SimpleVector2D::setAngle(float ang) {
	this->angle = ang;
	this->xComp = this->magnitude * std::cos(ang);
	this->yComp = this->magnitude * std::sin(ang);
}

void SimpleVector2D::changeMagnitude(float delta) {
	if (this->magnitude == 0) [[unlikely]] {
		setMagnitude(delta);
	} else if (this->magnitude + delta <= 0) [[unlikely]] {
		this->magnitude = 0;
		this->xComp = 0;
		this->yComp = 0;
	} else {
		this->xComp *= (this->magnitude + delta) / this->magnitude;
		this->yComp *= (this->magnitude + delta) / this->magnitude;
		this->magnitude += delta;
	}
}

void SimpleVector2D::changeAngle(float delta) {
	this->setAngle(this->angle + delta);
}

void SimpleVector2D::multiplyMagnitude(float scale) {
	if (scale <= 0) [[unlikely]] {
		this->magnitude = 0;
		this->xComp = 0;
		this->yComp = 0;
	} else {
		this->magnitude *= scale;
		this->xComp *= scale;
		this->yComp *= scale;
	}
}

void SimpleVector2D::scaleAndRotate(float scale, float a) {
	this->angle += a;
	this->magnitude *= scale;
	this->xComp = this->magnitude * std::cos(this->angle);
	this->yComp = this->magnitude * std::sin(this->angle);

	//would it be more efficient to use a scale and rotate matrix? probably not, there are more sines and cosines
	/*
	const float oldXComp = this->xComp;
	const float oldYComp = this->yComp;
	this->xComp = std::cos(a) * (scale*oldXComp) - std::sin(a) * (scale*oldYComp);
	this->yComp = std::sin(a) * (scale*oldXComp) + std::cos(a) * (scale*oldYComp);
	this->angle += a;
	this->magnitude *= scale;
	*/
}

SimpleVector2D SimpleVector2D::operator+(const SimpleVector2D& other) {
	SimpleVector2D result(xComp + other.xComp, yComp + other.yComp);
	return result;
}

SimpleVector2D SimpleVector2D::operator-(const SimpleVector2D& other) {
	SimpleVector2D result(xComp - other.xComp, yComp - other.yComp);
	return result;
}

std::ostream& operator<<(std::ostream& os, const SimpleVector2D& v) {
	return os << "<" << v.xComp << ", " << v.yComp << ">";
}

float SimpleVector2D::dotProduct(const SimpleVector2D& v1, const SimpleVector2D& v2) {
	//dot product definition: v1.x*v2.x + v1.y*v2.y = |v1||v2|cos(theta)
	//note: arccos cannot be negative
	return v1.getXComp()*v2.getXComp() + v1.getYComp()*v2.getYComp();
}

float SimpleVector2D::crossProduct(const SimpleVector2D& v1, const SimpleVector2D& v2) {
	//cross product definition: |v1 cross v2| = |v1||v2|sin(theta)
	//note: abs(arcsin) cannot be > PI/2
	//|i    j    k   |
	//|v1.x v1.y v1.z|
	//|v2.x v2.y v2.z|
	//v1.z = v2.z = 0, so
	//det = v1.x*v2.y - v2.x*v1.y
	return v1.getXComp()*v2.getYComp() - v2.getXComp()*v1.getYComp();
}

//finds the angle between the two vectors by the dot product and the cross product
//uses cross product for sign and dot product for angle
float SimpleVector2D::angleBetween(const SimpleVector2D& v1, const SimpleVector2D& v2) {
	/*
	float magnitudes = v1.getMagnitude() * v2.getMagnitude();
	float crossAngle = std::asin(SimpleVector2D::crossProduct(v1, v2) / magnitudes);
	float dotAngle = std::acos(SimpleVector2D::dotProduct(v1, v2) / magnitudes);
	*/
	//to account for some vectors having zero magnitude, set magnitude to 1:
	SimpleVector2D adjusted_v1 = MakeVector_Angle(v1.getAngle(), 1.0f);
	SimpleVector2D adjusted_v2 = MakeVector_Angle(v2.getAngle(), 1.0f);
	float crossAngle = std::asin(std::clamp<float>(SimpleVector2D::crossProduct(adjusted_v1, adjusted_v2), -1, 1));
	float dotAngle = std::acos(std::clamp<float>(SimpleVector2D::dotProduct(adjusted_v1, adjusted_v2), -1, 1));
	//clamping is unfortunately needed because float math; example: patrolling turrets could be unable to move forward due to dotAngle being NaN from acos(~1.001)
	return std::copysign(dotAngle, crossAngle);
}
