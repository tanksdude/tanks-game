#pragma once
#include <string>

std::string numToHex(unsigned char); //not needed anymore
std::string toUpperCase(std::string);
std::string toLowerCase(std::string);

double findMax(double*, int len);
double findMin(double*, int len);
int findMaxIndex(double*, int len);
int findMinIndex(double*, int len);

int constrain_i(int value, int min, int max);
double constrain_d(double value, double min, double max);

bool XOR(bool, bool);
double randFunc(); //[0,1)

//algorithm from https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
bool pointInPolygon(int vertNum, double* vertX, double* vertY, double testX, double testY);
bool pointInPolygon(int vertNum, float* vertX, float* vertY, float testX, float testY);

template<typename T>
bool isInArray(T find, T* arr, int length) {
	for (int i = 0; i < length; i++) {
		if (arr[i] == find) {
			return true;
		}
	}
	return false;
}
