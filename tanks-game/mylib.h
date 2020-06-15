#pragma once
#include <string>

std::string numToHex(unsigned char); //not needed anymore
std::string toUpperCase(std::string);
std::string toLowerCase(std::string);

double findMax(double*, int len);
double findMin(double*, int len);
int findMaxIndex(double*, int len);
int findMinIndex(double*, int len);
//std::max_element and std::min_element exist

template<typename T>
T constrain(T value, T min, T max);
//as of C++17, <algorithm> contains std::clamp, which does what I need
//however, I don't know how to change the C++ version in Visual Studio, if it's even possible, and also it's not vital to the project

bool XOR(bool, bool);
double randFunc(); //[0,1)
double randFunc2(); //[0,1]

//algorithm from https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
bool pointInPolygon(int vertNum, double* vertX, double* vertY, double testX, double testY);
bool pointInPolygon(int vertNum, float* vertX, float* vertY, float testX, float testY);

template<typename T>
bool isInArray(T find, T* arr, int length);
//it's just a linear search, which could be accomplished by std::find

template<typename T = double>
int weightedSelect(const T* weights, int num);
