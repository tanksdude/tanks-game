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
	for (short i = 0; i < stuff.size(); i++) {
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
	for (short i = 0; i < stuff.size(); i++) {
		if (stuff[i] >= 65 && stuff[i] <= 90) {
			letters += (stuff[i] + 32);
		}
		else {
			letters += stuff[i];
		}
	}
	return letters;
}