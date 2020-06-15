#include "mylib.h"
#include <sstream> //stringstream for hex conversion
#include <iostream>

std::string numToHex(unsigned char x) {
	std::stringstream stream;
	stream << std::hex << x;
	std::string s(stream.str());
	if (s.size() < 2) {
		s = "0" + s;
	}
	return toUpperCase(s);
}

std::string toUpperCase(std::string stuff) {
	std::string letters = "";
	for (int i = 0; i < stuff.size(); i++) {
		if (stuff[i] >= 97 && stuff[i] <= 122) {
			letters += (stuff[i] - 32);
		} else {
			letters += stuff[i];
		}
	}
	return letters;
}

std::string toLowerCase(std::string stuff) {
	std::string letters = "";
	for (int i = 0; i < stuff.size(); i++) {
		if (stuff[i] >= 65 && stuff[i] <= 90) {
			letters += (stuff[i] + 32);
		}
		else {
			letters += stuff[i];
		}
	}
	return letters;
}

double findMax(double* arr, int len) { //precondition: length of array > 0
	double max = arr[0];
	for (int i = 1; i < len; i++) {
		if (arr[i] > max) {
			max = arr[i];
		}
	}
	return max;
}

double findMin(double* arr, int len) { //precondition: length of array > 0
	double min = arr[0];
	for (int i = 1; i < len; i++) {
		if (arr[i] < min) {
			min = arr[i];
		}
	}
	return min;
}

int findMaxIndex(double* arr, int len) { //precondition: length of array > 0
	int index = 0;
	for (int i = 1; i < len; i++) {
		if (arr[i] > arr[index]) {
			index = i;
		}
	}
	return index;
}

int findMinIndex(double* arr, int len) { //precondition: length of array > 0
	int index = 0;
	for (int i = 1; i < len; i++) {
		if (arr[i] < arr[index]) {
			index = i;
		}
	}
	return index;
}

template<typename T>
T constrain(T value, T min, T max) {
	if (value < min) {
		return min;
	}
	if (value > max) {
		return max;
	}
	return value;
}
template int constrain<int>(int, int, int);
template long constrain<long>(long, long, long);
template float constrain<float>(float, float, float);
template double constrain<double>(double, double, double);
//normally, these template specializations should go in the header, but for some reason they're allowed here
//(behind the scenes, I understand what's happening, mostly, but it's still moderately cool)

bool XOR(bool a, bool b) {
	return ((a || b) && !(a && b));
}

double randFunc() {
	return double(rand()) / double(RAND_MAX+1);
}

double randFunc2() {
	return double(rand()) / double(RAND_MAX);
}

//algorithm from https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
/*
Copyright (c) 1970-2003, Wm. Randolph Franklin

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

1.  Redistributions of source code must retain the above copyright notice, this 
    list of conditions and the following disclaimers.
2.  Redistributions in binary form must reproduce the above copyright notice in 
    the documentation and/or other materials provided with the distribution.
3.  The name of W. Randolph Franklin may not be used to endorse or promote 
    products derived from this Software without specific prior written permission. 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
*/
bool pointInPolygon(int vertNum, double* vertX, double* vertY, double testX, double testY) {
	int i, j, c = 0;
	for (i = 0, j = vertNum-1; i < vertNum; j = i++) {
		if ( ((vertY[i]>testY) != (vertY[j]>testY)) &&
		     (testX < (vertX[j]-vertX[i]) * (testY-vertY[i]) / (vertY[j]-vertY[i]) + vertX[i]) )
			c = !c;
	}
	return c;
}
bool pointInPolygon(int vertNum, float* vertX, float* vertY, float testX, float testY) {
	int i, j, c = 0;
	for (i = 0, j = vertNum-1; i < vertNum; j = i++) {
		if ( ((vertY[i]>testY) != (vertY[j]>testY)) &&
		     (testX < (vertX[j]-vertX[i]) * (testY-vertY[i]) / (vertY[j]-vertY[i]) + vertX[i]) )
			c = !c;
	}
	return c;
}

template<typename T>
bool isInArray(T find, T* arr, int length) {
	for (int i = 0; i < length; i++) {
		if (arr[i] == find) {
			return true;
		}
	}
	return false;
}
template bool isInArray<double>(double, double*, int); //I don't think this one is used
template bool isInArray<int>(int, int*, int);

//algorithm from https://forum.unity.com/threads/weighted-random-selection.221890/ and https://forum.unity.com/threads/randomising-shuffling-library.29977/
template<typename T>
int weightedSelect(const T* weights, int num) {
	//TODO: unit tests
	T weightTotal = 0;
	for (int i = 0; i < num; i++) {
		weightTotal += weights[i];
	}
	T targetWeight = randFunc() * weightTotal;
	T newWeight = 0;
	int newIndex = 0;
	while (targetWeight >= newWeight) {
		newWeight += weights[newIndex];
		newIndex++;
	}
	return newIndex-1;
}
template int weightedSelect(const double*, int);
template int weightedSelect(const float*, int);
