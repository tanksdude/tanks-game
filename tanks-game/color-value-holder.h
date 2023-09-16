#pragma once

struct ColorValueHolder {
	friend class ColorMixer;

protected:
	float R;
	float G;
	float B;
	float A;

public:
	inline float getRf() const { return R; }
	inline float getGf() const { return G; }
	inline float getBf() const { return B; }
	inline float getAf() const { return A; } //no real point

	ColorValueHolder(float, float, float);
	ColorValueHolder(float, float, float, float);
	ColorValueHolder() { R = 0; G = 0; B = 0; A = 1; }
	//ColorValueHolder(const ColorValueHolder&); //implicitly generated
};
