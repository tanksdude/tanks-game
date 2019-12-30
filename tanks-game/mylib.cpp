#pragma once
#include "mylib.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

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

/*
template <typename numType>
numType constrain(numType value, numType min, numType max) {
	return ((value < min ? min : value) > max ? max : value);
	//is separating it from ternary operators going to be more efficient? does it even matter?
}
*/