#pragma once

#include <chrono>
#include <vector>
#include <string>

class Diagnostics {
private:
	static std::vector<std::chrono::time_point<std::chrono::steady_clock>> times;
	static std::vector<std::string> timeNames;
	static bool currentlyTiming;
public:
	static std::chrono::time_point<std::chrono::steady_clock> getTime();
	static long long getDiff(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> end);

	static void startTiming();
	static void endTiming();
	static void addName(std::string);
	static void clearTimes(); //(and names)
	static void printTimings();
};