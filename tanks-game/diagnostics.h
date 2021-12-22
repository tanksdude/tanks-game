#pragma once
#include <chrono>
#include <vector>
#include <string>
#include <unordered_map>
#include "color-value-holder.h"

#include "vertex-array.h"
#include "vertex-buffer.h"
#include "index-buffer.h"

class Diagnostics {
private:
	static std::vector<std::chrono::time_point<std::chrono::steady_clock>> times;
	static std::vector<std::string> timeNames;
	static bool currentlyTiming;

	struct GraphData {
		std::vector<long double> data; //should use a list but iterators are so messy
		std::string name;
		ColorValueHolder color;
		GraphData(std::string, ColorValueHolder);
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
	static void drawGraphTimes_data();

private:
	static VertexArray* graph_va;
	static VertexBuffer* graph_vb;
	//static IndexBuffer* graph_ib;
	static VertexArray* data_va;
	static VertexBuffer* data_vb;
	static int data_vb_length;
	static bool initialized_GPU;

	static bool initializeGPU();
	static void streamDataGPU(const std::vector<long double>&);
	static bool uninitializeGPU();

public:
	static void Initialize();

	static std::chrono::time_point<std::chrono::steady_clock> getTime();
	static long double getDiff(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> end);

	static void startTiming(std::string);
	static void endTiming();
	static void clearTimes(); //(and names)
	static void printTimings(); //plain milliseconds
	static void printPreciseTimings(); //microsecond accuracy

	static void declareGraph(std::string name, ColorValueHolder color);
	static void pushGraphTime(std::string name, long double time);
	static void clearGraph(std::string name);
	static void clearGraph(); //all graphs
	static void drawGraphTimes(std::string name);
	static void drawGraphTimes(); //all graphs

	static void setGraphYOffset(double);

private:
	Diagnostics() {}
	Diagnostics(const Diagnostics&) {}
};
