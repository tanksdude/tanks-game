#include "colorvalueholder.h"
#include "mylib.h"

ColorValueHolder::ColorValueHolder(float r, float g, float b) : ColorValueHolder(r, g, b, 1) {}

ColorValueHolder::ColorValueHolder(float r, float g, float b, float a) {
	R = constrain<float>(r, 0.0, 1.0);
	G = constrain<float>(g, 0.0, 1.0);
	B = constrain<float>(b, 0.0, 1.0);
	A = constrain<float>(a, 0.0, 1.0);
}

ColorValueHolder::ColorValueHolder(const ColorValueHolder& other) {
	this->R = other.R;
	this->G = other.G;
	this->B = other.B;
	this->A = other.A;
}
