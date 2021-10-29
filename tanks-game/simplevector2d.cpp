#include "simplevector2d.h"
#include <math.h>
#include "constants.h"

SimpleVector2D::SimpleVector2D(float xComp, float yComp) {
	this->xComp = xComp;
	this->yComp = yComp;
	this->angle = atan2(yComp, xComp);
	this->magnitude = sqrt(xComp*xComp + yComp*yComp);
}

SimpleVector2D::SimpleVector2D(float angle, float magnitude, bool angle_magnitude) {
	this->angle = angle;
	this->magnitude = magnitude;
	this->xComp = magnitude * cos(angle);
	this->yComp = magnitude * sin(angle);
}

SimpleVector2D::SimpleVector2D(const SimpleVector2D& other) {
	this->xComp = other.xComp;
	this->yComp = other.yComp;
	this->angle = other.angle;
	this->magnitude = other.magnitude;
}

void SimpleVector2D::setMagnitude(float magnitude) {
	if (magnitude < 0) {
		magnitude = 0;
	}
	this->magnitude = magnitude;
	this->xComp = magnitude * cos(angle);
	this->yComp = magnitude * sin(angle);
}

void SimpleVector2D::setAngle(float angle) {
	this->angle = angle;
	this->xComp = this->magnitude * cos(angle);
	this->yComp = this->magnitude * sin(angle);
}

void SimpleVector2D::changeMagnitude(float delta) {
	this->setMagnitude(this->magnitude + delta);
}

void SimpleVector2D::changeAngle(float delta) {
	this->setAngle(this->angle + delta);
}

SimpleVector2D SimpleVector2D::operator+(const SimpleVector2D& other) {
	SimpleVector2D result(xComp + other.xComp, yComp + other.yComp);
	//if (result.magnitude < 0) {
	//	result.magnitude *= -1;
	//	result.angle += PI;
	//}
	return result;
}

SimpleVector2D SimpleVector2D::operator-(const SimpleVector2D& other) {
	SimpleVector2D result(xComp - other.xComp, yComp - other.yComp);
	//if (result.magnitude < 0) {
	//	result.magnitude *= -1;
	//	result.angle += PI;
	//}
	return result;
}

std::ostream& operator<<(std::ostream& os, const SimpleVector2D& v) {
	return os << v.xComp << " " << v.yComp;
}

/*
//finds the angle between the two vectors by the dot product (can be negative*)
//*arccos cannot be negative, so the returned angle cannot be negative...
float SimpleVector2D::angleBetween(const SimpleVector2D& v1, const SimpleVector2D& v2) {
	//dot product: v1.x*v2.x + v1.y*v2.y = |v1||v2|cos(theta)
	//theta = arccos((v1.x*v2.x + v1.y*v2.y) / (|v1||v2|)))
	return acos((v1.getXComp()*v2.getXComp() + v1.getYComp()*v2.getYComp()) / (v1.getMagnitude() * v2.getMagnitude()));
}
*/

/*
//finds the angle between the two vectors by the cross product (can be negative)
//problem: does not handle |theta| > PI/2
float SimpleVector2D::angleBetween(const SimpleVector2D& v1, const SimpleVector2D& v2) {
	//cross product: |v1 cross v2| = |v1||v2|sin(theta)
	//|i    j    k   |
	//|v1.x v1.y v1.z|
	//|v2.x v2.y v2.z|
	//v1.z = v2.z = 0, so
	//det = v1.x*v2.y - v2.x-v1.y
	//theta = arcsin((v1.x*v2.y - v2.x*v1.y) / (|v1||v2|)))
	return = asin((v1.getXComp()*v2.getYComp() - v2.getXComp()*v1.getYComp()) / (v1.getMagnitude() * v2.getMagnitude()));
}
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
	float crossAngle = asin(SimpleVector2D::crossProduct(adjusted_v1, adjusted_v2));
	float dotAngle = acos(SimpleVector2D::dotProduct(adjusted_v1, adjusted_v2));
	return copysign(dotAngle, crossAngle); //this function is surprisingly useful!
}
