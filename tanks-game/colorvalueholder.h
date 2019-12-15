#pragma once
#include <string>
#include "mylib.h"

class ColorValueHolder{
public:
	friend class ColorMixer;
	unsigned char R;
	unsigned char G;
	unsigned char B;
	std::string hex;

public:
	ColorValueHolder(unsigned char, unsigned char, unsigned char);
	ColorValueHolder() { R = 0; G = 0; B = 0; }
	std::string getHex();
	unsigned char* getRGBv();

	unsigned char getR(){return R;}
	unsigned char getG(){return G;}
	unsigned char getB(){return B;}

	float getRf(){return R/255.0;}
	float getGf(){return G/255.0;}
	float getBf(){return B/255.0;}
};

