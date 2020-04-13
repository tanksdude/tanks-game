#pragma once

#include "diagnostics.h"
#include <iostream>

std::vector<std::chrono::time_point<std::chrono::steady_clock>> Diagnostics::times;
std::vector<std::string> Diagnostics::timeNames;
bool Diagnostics::currentlyTiming = false;

std::chrono::time_point<std::chrono::steady_clock> Diagnostics::getTime() {
	return std::chrono::steady_clock::now();
}

long double Diagnostics::getDiff(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> end) {
	return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0; //milliseconds
}

void Diagnostics::startTiming() {
	if (!currentlyTiming) {
		times.push_back(std::chrono::steady_clock::now());
	}
	currentlyTiming = true;
}

void Diagnostics::endTiming() {
	if (currentlyTiming) {
		times.push_back(std::chrono::steady_clock::now());
	}
	currentlyTiming = false;
}

void Diagnostics::addName(std::string s) {
	timeNames.push_back(s);
}

void Diagnostics::startTiming(std::string s) {
	startTiming();
	addName(s);
}

void Diagnostics::clearTimes() {
	times.clear();
	currentlyTiming = false;
	timeNames.clear();
}

void Diagnostics::printTimings() {
	for (int i = 0; i < times.size()/2, i < timeNames.size(); i++) {
		std::cout << timeNames[i] << ": " << (long long)getDiff(times[i*2], times[i*2+1]) << "ms" << std::endl;
	}
}

void Diagnostics::printPreciseTimings() {
	for (int i = 0; i < times.size()/2, i < timeNames.size(); i++) {
		std::cout << timeNames[i] << ": " << getDiff(times[i*2], times[i*2+1]) << "ms" << std::endl;
	}
}