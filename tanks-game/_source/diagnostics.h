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

	typedef double time_float;
	struct GraphData {
		std::vector<time_float> data; //should use a linked list but iterators are messy
		std::string name;
		ColorValueHolder color;
		GraphData(std::string, const ColorValueHolder&);
		GraphData() { name = ""; color = ColorValueHolder(0, 0, 0); }
	};

	static std::vector<GraphData> graphTimes;
	static std::unordered_map<std::string, int> graphNameToIndex;
	static int maxGraphTimes;
	static float graphLength;
	static float graphHeight;
	static float graphXOffset;
	static float graphYOffset;

	static void drawGraphTimes_graph();
	static void drawGraphTimes_data(std::string name);
	static void drawGraphTimes_data();

public:
	static void Initialize();

	static inline std::chrono::time_point<std::chrono::steady_clock> getTime() {
		return std::chrono::steady_clock::now();
	}
	static inline time_float getDiff(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> end) {
		return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / time_float(1000.0f); //milliseconds
	}

	static void startTiming(std::string);
	static void endTiming();
	static void clearTimes(); //(and names)
	static void printTimings(); //plain milliseconds
	static void printPreciseTimings(); //microsecond accuracy

	static void declareGraph(std::string name, const ColorValueHolder& color);
	static void pushGraphTime(std::string name, time_float time);
	static void clearGraph(std::string name);
	static void clearGraph(); //all graphs
	static void drawGraphTimes(std::string name);
	static void drawGraphTimes(); //all graphs

	static void setGraphYOffset(float);
	static void pushGraphSumTime(std::string name);

private:
	Diagnostics() = delete;
	Diagnostics(const Diagnostics&) = delete;
};
