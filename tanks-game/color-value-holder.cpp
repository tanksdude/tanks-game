#include "color-value-holder.h"
#include <algorithm> //std::clamp

ColorValueHolder::ColorValueHolder(float r, float g, float b) : ColorValueHolder(r, g, b, 1) {}

ColorValueHolder::ColorValueHolder(float r, float g, float b, float a) {
	R = std::clamp<float>(r, 0.0f, 1.0f);
	G = std::clamp<float>(g, 0.0f, 1.0f);
	B = std::clamp<float>(b, 0.0f, 1.0f);
	A = std::clamp<float>(a, 0.0f, 1.0f);
}

ColorValueHolder::ColorValueHolder(const ColorValueHolder& other) {
	this->R = other.R;
	this->G = other.G;
	this->B = other.B;
	this->A = other.A;
}
