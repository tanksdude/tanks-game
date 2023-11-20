#pragma once
#include <algorithm> //std::min_element, std::max_element, std::find

template<typename T>
inline T findMin(const T* arr, int len) { return *std::min_element(arr, arr + len); }
template<typename T>
inline T findMax(const T* arr, int len) { return *std::max_element(arr, arr + len); }
template<typename T>
inline int findMinIndex(const T* arr, int len) { return std::distance(arr, std::min_element(arr, arr + len)); }
template<typename T>
inline int findMaxIndex(const T* arr, int len) { return std::distance(arr, std::max_element(arr, arr + len)); }

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
