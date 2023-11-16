#pragma once
#include <string>
#include <algorithm> //std::find

std::string numToHex(unsigned char); //not needed anymore
std::string toUpperCase(std::string);
std::string toLowerCase(std::string);

double findMax(const double*, int len);
double findMin(const double*, int len);
int findMaxIndex(const double*, int len);
int findMinIndex(const double*, int len);
//std::max_element and std::min_element exist

/*
template<typename T>
T constrain(T value, T min, T max);
//as of C++17, <algorithm> contains std::clamp, which does what I need
//however, I don't know how to change the C++ version in Visual Studio, if it's even possible, and also it's not vital to the project
*/
//update: so there is a way, and I feel like this wasn't always possible which is why I never investigated further:
//C++ Language Standard under General Properties, previously set to "Default (ISO C++14 Standard)", can be changed
//changed to C++20 ("ISO C++20 Standard (/std:c++20)") because I want attributes (mainly [[likely]] and [[unlikely]])
//apparently it's been possible since VS2017, wish I knew https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170

constexpr bool XOR(bool a, bool b) {
	return (a != b);
	//return ((a || b) && !(a && b));
}

//algorithm from https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
bool pointInPolygon(int vertNum, const double* vertX, const double* vertY, double testX, double testY);
bool pointInPolygon(int vertNum, const float* vertX, const float* vertY, float testX, float testY);

template<typename T>
inline bool isInArray(T value, const T* arr, int length) {
	return (std::find(arr, arr + length, value) != (arr + length));
}

template<typename T>
int weightedSelect(const T* weights, int num);
