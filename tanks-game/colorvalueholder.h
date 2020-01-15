#pragma once

class ColorValueHolder{
public:
	friend class ColorMixer;
	float R; //very little point in storing them as doubles when they'd just have to be casted to floats to be drawn
	float G;
	float B;
	float A;

public:
	ColorValueHolder(float, float, float);
	ColorValueHolder(float, float, float, float);
	ColorValueHolder() { R = 0; G = 0; B = 0; A = 1; }

	float getRf(){return R;}
	float getGf(){return G;}
	float getBf(){return B;}
	float getAf(){return A;} //no real point
};
