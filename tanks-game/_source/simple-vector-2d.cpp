#include "simple-vector-2d.h"

#include "constants.h"
#include <cmath>
#include <algorithm> //std::clamp

SimpleVector2D::SimpleVector2D(float xComp, float yComp) {
	this->xComp = xComp;
	this->yComp = yComp;
	this->angle = atan2(yComp, xComp);
	this->magnitude = sqrt(xComp*xComp + yComp*yComp);
	//hypot() is slow, don't use it (unless you're dealing with massive values that could overflow)
	//https://stackoverflow.com/questions/32435796/when-to-use-stdhypotx-y-over-stdsqrtxx-yy
}

SimpleVector2D::SimpleVector2D(float angle, float magnitude, bool angle_magnitude) {
	this->angle = angle;
	this->magnitude = magnitude;
	this->xComp = magnitude * cos(angle);
	this->yComp = magnitude * sin(angle);
}

SimpleVector2D::SimpleVector2D(float xComp, float yComp, float angle, float magnitude) {
	this->xComp = xComp;
	this->yComp = yComp;
	this->angle = angle;
	this->magnitude = magnitude;
}

/*
SimpleVector2D::SimpleVector2D(const SimpleVector2D& other) {
	this->xComp = other.xComp;
	this->yComp = other.yComp;
	this->angle = other.angle;
	this->magnitude = other.magnitude;
}
*/

SimpleVector2D SimpleVector2D::MakeVector_Components(float xComp, float yComp) {
	return { xComp, yComp };
}

SimpleVector2D SimpleVector2D::MakeVector_Angle(float angle, float magnitude) {
	return { angle, magnitude, true };
}

SimpleVector2D SimpleVector2D::MakeVector_ComponentsMagnitude(float xComp, float yComp, float magnitude) {
	float angle = atan2(yComp, xComp);
	float newXComp = magnitude * cos(angle);
	float newYComp = magnitude * sin(angle);
	return { newXComp, newYComp, angle, magnitude };
}

void SimpleVector2D::setMagnitude(float mag) {
	if (mag <= 0) {
		this->magnitude = 0;
		this->xComp = 0;
		this->yComp = 0;
	} else {
		this->magnitude = mag;
		this->xComp = mag * cos(this->angle);
		this->yComp = mag * sin(this->angle);
	}
}

void SimpleVector2D::setAngle(float ang) {
	this->angle = ang;
	this->xComp = this->magnitude * cos(ang);
	this->yComp = this->magnitude * sin(ang);
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
	this->xComp = this->magnitude * cos(this->angle);
	this->yComp = this->magnitude * sin(this->angle);

	//would it be more efficient to use a scale and rotate matrix? probably not, there are more sines and cosines
	/*
	const double oldxComp = this->xComp;
	const double oldyComp = this->yComp;
	this->xComp = cos(a) * (scale*oldxComp) - sin(a) * (scale*oldyComp);
	this->yComp = sin(a) * (scale*oldxComp) + cos(a) * (scale*oldyComp);
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

/*
//finds the angle between the two vectors by the dot product (can be negative)
float SimpleVector2D::angleBetween(const SimpleVector2D& v1, const SimpleVector2D& v2) {
	//dot product: v1.x*v2.x + v1.y*v2.y = |v1||v2|cos(theta)
	//theta = arccos((v1.x*v2.x + v1.y*v2.y) / (|v1||v2|)))
	return acos((v1.getXComp()*v2.getXComp() + v1.getYComp()*v2.getYComp()) / (v1.getMagnitude() * v2.getMagnitude()));
}
//problem: arccos cannot be negative, so the returned angle cannot be negative...
*/

/*
//finds the angle between the two vectors by the cross product (can be negative)
float SimpleVector2D::angleBetween(const SimpleVector2D& v1, const SimpleVector2D& v2) {
	//cross product: |v1 cross v2| = |v1||v2|sin(theta)
	//|i    j    k   |
	//|v1.x v1.y v1.z|
	//|v2.x v2.y v2.z|
	//v1.z = v2.z = 0, so
	//det = v1.x*v2.y - v2.x*v1.y
	//theta = arcsin((v1.x*v2.y - v2.x*v1.y) / (|v1||v2|)))
	return = asin((v1.getXComp()*v2.getYComp() - v2.getXComp()*v1.getYComp()) / (v1.getMagnitude() * v2.getMagnitude()));
}
//problem: does not handle |theta| > PI/2
*/

float SimpleVector2D::dotProduct(const SimpleVector2D& v1, const SimpleVector2D& v2) {
	return v1.getXComp()*v2.getXComp() + v1.getYComp()*v2.getYComp();
}

float SimpleVector2D::crossProduct(const SimpleVector2D& v1, const SimpleVector2D& v2) {
	return v1.getXComp()*v2.getYComp() - v2.getXComp()*v1.getYComp();
}

//finds the angle between the two vectors by the dot product and the cross product
//uses cross product for sign and dot product for angle
float SimpleVector2D::angleBetween(const SimpleVector2D& v1, const SimpleVector2D& v2) {
	/*
	float magnitudes = v1.getMagnitude() * v2.getMagnitude();
	float crossAngle = asin(SimpleVector2D::crossProduct(v1, v2) / magnitudes);
	float dotAngle = acos(SimpleVector2D::dotProduct(v1, v2) / magnitudes);
	*/
	//to account for the fact some vectors have zero magnitude, set magnitude to 1:
	SimpleVector2D adjusted_v1(v1), adjusted_v2(v2);
	adjusted_v1.setMagnitude(1); adjusted_v2.setMagnitude(1);
	float crossAngle = asin(std::clamp<float>(SimpleVector2D::crossProduct(adjusted_v1, adjusted_v2), -1, 1));
	float dotAngle = acos(std::clamp<float>(SimpleVector2D::dotProduct(adjusted_v1, adjusted_v2), -1, 1));
	//clamping is unfortunately needed because float math; example: patrolling turrets could be unable to move forward due to dotAngle being NaN from acos(~1.001)
	return copysign(dotAngle, crossAngle); //this function is surprisingly helpful!
}
