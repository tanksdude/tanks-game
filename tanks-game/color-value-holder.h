#pragma once

struct ColorValueHolder {
	friend class ColorMixer;

protected:
	float R; //very little point in storing them as doubles when they'd just have to be casted to floats to be drawn
	float G;
	float B;
	float A;

public:
	float getRf() const { return R; }
	float getGf() const { return G; }
	float getBf() const { return B; }
	float getAf() const { return A; } //no real point

	ColorValueHolder(float, float, float);
	ColorValueHolder(float, float, float, float);
	ColorValueHolder() { R = 0; G = 0; B = 0; A = 1; }
	ColorValueHolder(const ColorValueHolder&);
};
