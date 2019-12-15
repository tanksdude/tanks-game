#pragma once
#include "colorvalueholder.h"
#include "mylib.h"
#include <string>

ColorValueHolder::ColorValueHolder(unsigned char r, unsigned char g, unsigned char b){
	R = r;
	G = g;
	B = b;
	hex = "#" + numToHex(R) + numToHex(G) + numToHex(B);
}

std::string ColorValueHolder::getHex() {
	return hex;
}

unsigned char* ColorValueHolder::getRGBv() {
	unsigned char* arr = new unsigned char[3];
	arr[0] = R;
	arr[1] = G;
	arr[2] = B;
	return arr;
}
