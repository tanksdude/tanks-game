#pragma once
#include <string>

std::string numToHex(unsigned char); //not needed anymore
std::string toUpperCase(std::string);
std::string toLowerCase(std::string);

double findMax(double*, int len);
double findMin(double*, int len);
int findMaxIndex(double*, int len);
int findMinIndex(double*, int len);

//C++ is dumb and needs the complete definition of template functions in header files
//my understanding of the reason: the compiler creates the function code when the function is called, so it needs to know exactly what can happen
template <typename numType>
numType constrain(numType value, numType min, numType max) {
	return ((value < min ? min : value) > max ? max : value);
	//is separating it from ternary operators going to be more efficient? does it even matter?
}