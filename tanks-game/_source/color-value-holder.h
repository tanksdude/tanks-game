#pragma once

struct ColorValueHolder {
	friend class ColorMixer;

protected:
	float R;
	float G;
	float B;
	float A;

public:
	inline float getRf() const noexcept { return R; }
	inline float getGf() const noexcept { return G; }
	inline float getBf() const noexcept { return B; }
	inline float getAf() const noexcept { return A; } //transparency not used

	ColorValueHolder(float, float, float);
	ColorValueHolder(float, float, float, float);
	ColorValueHolder() { R = 0; G = 0; B = 0; A = 1; }
};
