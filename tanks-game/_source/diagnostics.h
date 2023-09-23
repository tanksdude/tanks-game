#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>

#include "color-value-holder.h"

class Diagnostics {
private:
	static std::vector<std::chrono::time_point<std::chrono::steady_clock>> times;
	static std::vector<std::string> timeNames;
	static bool currentlyTiming;

	struct GraphData {
		std::vector<long double> data; //should use a linked list but iterators are so messy
		//long doubles are utterly unnecessary, but it's the time difference units from std::chrono
		std::string name;
		ColorValueHolder color;
		GraphData(std::string, const ColorValueHolder&);
		GraphData() { name = ""; color = ColorValueHolder(0, 0, 0); }
	};

	static std::vector<GraphData> graphTimes;
	static std::unordered_map<std::string, int> graphNameToIndex;
	static int maxGraphTimes;
	static double graphLength;
	static double graphHeight;
	static double graphXOffset;
	static double graphYOffset;

	static void drawGraphTimes_graph();
	static void drawGraphTimes_data(std::string name);
	static inline void drawGraphTimes_data();

public:
	static void Initialize();

	static std::chrono::time_point<std::chrono::steady_clock> getTime();
	static long double getDiff(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> end);

	static void startTiming(std::string);
	static void endTiming();
	static void clearTimes(); //(and names)
	static void printTimings(); //plain milliseconds
	static void printPreciseTimings(); //microsecond accuracy

	static void declareGraph(std::string name, const ColorValueHolder& color);
	static void pushGraphTime(std::string name, long double time);
	static void clearGraph(std::string name);
	static void clearGraph(); //all graphs
	static void drawGraphTimes(std::string name);
	static void drawGraphTimes(); //all graphs

	static void setGraphYOffset(double);
	static void pushGraphSumTime(std::string name);

private:
	Diagnostics() = delete;
	Diagnostics(const Diagnostics&) = delete;
};
