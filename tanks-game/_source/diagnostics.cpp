#include "diagnostics.h"

#include "constants.h"
#include <algorithm> //std::min, std::max
#include <iostream>

#include "renderer.h"
#include "simple-vector-2d.h"

std::vector<std::chrono::time_point<std::chrono::steady_clock>> Diagnostics::times;
std::vector<std::string> Diagnostics::timeNames;
bool Diagnostics::currentlyTiming = false;

std::vector<Diagnostics::GraphData> Diagnostics::graphTimes;
std::unordered_map<std::string, int> Diagnostics::graphNameToIndex;
int Diagnostics::maxGraphTimes = 200;
double Diagnostics::graphLength = GAME_WIDTH/4;
double Diagnostics::graphHeight = GAME_HEIGHT/4;
double Diagnostics::graphXOffset = 0;
double Diagnostics::graphYOffset = 0;

Diagnostics::GraphData::GraphData(std::string n, const ColorValueHolder& c) {
	this->name = n;
	this->color = c;
}

void Diagnostics::setGraphYOffset(double y) {
	graphYOffset = y;
}

void Diagnostics::Initialize() {
	//nothing
}

std::chrono::time_point<std::chrono::steady_clock> Diagnostics::getTime() {
	return std::chrono::steady_clock::now();
}

long double Diagnostics::getDiff(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> end) {
	return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0; //milliseconds
}

void Diagnostics::startTiming(std::string s) {
	if (!currentlyTiming) {
		times.push_back(std::chrono::steady_clock::now());
		timeNames.push_back(s);
	}
	currentlyTiming = true;
}

void Diagnostics::endTiming() {
	if (currentlyTiming) {
		times.push_back(std::chrono::steady_clock::now());
	}
	currentlyTiming = false;
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

void Diagnostics::declareGraph(std::string name, const ColorValueHolder& color) {
	graphNameToIndex.insert({ name, graphTimes.size() });
	graphTimes.push_back(GraphData(name, color));
}

void Diagnostics::pushGraphTime(std::string name, long double time) {
	std::vector<long double>& currentGraph = graphTimes[graphNameToIndex[name]].data;

	currentGraph.push_back(time);
	while (currentGraph.size() > maxGraphTimes) {
		currentGraph.erase(currentGraph.begin());
	}

	if (maxGraphTimes <= 200) {
		graphLength = GAME_WIDTH/4;
	} else if (maxGraphTimes >= 2000) {
		graphLength = GAME_WIDTH/2;
	} else {
		graphLength = (maxGraphTimes-200)/2000.0 * (GAME_WIDTH/4) + GAME_WIDTH/4;
	}
}

void Diagnostics::clearGraph(std::string name) {
	graphTimes[graphNameToIndex[name]].data.clear();
}

void Diagnostics::clearGraph() {
	for (int i = 0; i < graphTimes.size(); i++) {
		graphTimes[i].data.clear();
	}
}

void Diagnostics::pushGraphSumTime(std::string name) {
	long double sum = 0;
	for (int i = 0; i < graphTimes.size(); i++) {
		if ((graphTimes[i].name == name) || (graphTimes[i].data.size() == 0)) {
			continue;
		}
		sum += graphTimes[i].data[graphTimes[i].data.size()-1];
		//technically not right; the proper implementation is to check everything that got updated and sum those
	}

	pushGraphTime(name, sum);
}

void Diagnostics::drawGraphTimes() {
	drawGraphTimes_graph();
	drawGraphTimes_data();
}

void Diagnostics::drawGraphTimes(std::string name) {
	drawGraphTimes_graph();
	drawGraphTimes_data(name);
}

void Diagnostics::drawGraphTimes_graph() {
	ColorValueHolder color = ColorValueHolder(0.75f, 0.75f, 0.75f);
	const float lineWidth = 1.0f;

	float coordsAndColor[] = {
		//outer
		(graphXOffset)             - lineWidth, (graphYOffset)             - lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //0
		(graphXOffset+graphLength) + lineWidth, (graphYOffset)             - lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //1
		(graphXOffset+graphLength) + lineWidth, (graphYOffset+graphHeight) + lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //2
		(graphXOffset)             - lineWidth, (graphYOffset+graphHeight) + lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //3

		//inner
		(graphXOffset)             + lineWidth, (graphYOffset)             + lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //4
		(graphXOffset+graphLength) - lineWidth, (graphYOffset)             + lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //5
		(graphXOffset+graphLength) - lineWidth, (graphYOffset+graphHeight) - lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf(), //6
		(graphXOffset)             + lineWidth, (graphYOffset+graphHeight) - lineWidth,   color.getRf(), color.getGf(), color.getBf(), color.getAf()  //7
	};
	unsigned int indices[] = { //trapezoids
		//bottom
		//0, 1, 5,
		//5, 4, 0,

		//right
		1, 2, 6,
		6, 5, 1,

		//left
		4, 7, 3,
		3, 0, 4,

		//top
		2, 3, 7,
		7, 6, 2
	};

	Renderer::SubmitBatchedDraw(coordsAndColor, (4+4) * (2+4), indices, (3*2) * 3);
	//would look better using rectangles but it's fine
}

void Diagnostics::drawGraphTimes_data(std::string name) {
	std::vector<long double>& graphData = graphTimes[graphNameToIndex[name]].data;
	if (graphData.size() < 2) { [[unlikely]]
		return;
	}

	ColorValueHolder color = graphTimes[graphNameToIndex[name]].color;
	const float lineWidth = 1.0f; //TODO: too thick?

	std::vector<float> positions; positions.reserve(graphData.size()*2);
	for (int i = 0; i < graphData.size(); i++) {
		positions.push_back(graphLength * (float(i)/float(maxGraphTimes-1)));
		positions.push_back(graphHeight * (graphData[i] * (1.0 / 10))); //==1 means it took a full frame (10ms)
	}

	float* coordsAndColor = new float[(graphData.size()-1)*4*(2+4)];
	unsigned int* indices = new unsigned int[(graphData.size()-1)*6];

	for (int j = 0; j < graphData.size()-1; j++) {
		const int startVertex = (j*4) * 6;
		const int startIndex = j*6;

		SimpleVector2D dist = SimpleVector2D(positions[(j+1)*2] - positions[j*2], positions[(j+1)*2+1] - positions[j*2+1]);
		SimpleVector2D distCW = SimpleVector2D(dist.getAngle() - PI/2, lineWidth, true);

		coordsAndColor[startVertex + 0*6]   = graphXOffset + positions[j*2]                     + distCW.getXComp();
		coordsAndColor[startVertex + 0*6+1] = graphYOffset + positions[j*2+1]                   + distCW.getYComp();
		coordsAndColor[startVertex + 1*6]   = graphXOffset + positions[j*2]   + dist.getXComp() + distCW.getXComp();
		coordsAndColor[startVertex + 1*6+1] = graphYOffset + positions[j*2+1] + dist.getYComp() + distCW.getYComp();
		coordsAndColor[startVertex + 2*6]   = graphXOffset + positions[j*2]   + dist.getXComp() - distCW.getXComp();
		coordsAndColor[startVertex + 2*6+1] = graphYOffset + positions[j*2+1] + dist.getYComp() - distCW.getYComp();
		coordsAndColor[startVertex + 3*6]   = graphXOffset + positions[j*2]                     - distCW.getXComp();
		coordsAndColor[startVertex + 3*6+1] = graphYOffset + positions[j*2+1]                   - distCW.getYComp();

		for (int k = 0; k < 4; k++) {
			coordsAndColor[startVertex + k*6+2] = color.getRf();
			coordsAndColor[startVertex + k*6+3] = color.getGf();
			coordsAndColor[startVertex + k*6+4] = color.getBf();
			coordsAndColor[startVertex + k*6+5] = color.getAf();
		}

		indices[startIndex + 0] = startVertex/6 + 0;
		indices[startIndex + 1] = startVertex/6 + 1;
		indices[startIndex + 2] = startVertex/6 + 2;
		indices[startIndex + 3] = startVertex/6 + 2;
		indices[startIndex + 4] = startVertex/6 + 3;
		indices[startIndex + 5] = startVertex/6 + 0;
	}

	Renderer::SubmitBatchedDraw(coordsAndColor, (graphData.size()-1)*4*(2+4), indices, (graphData.size()-1)*6);
	delete[] coordsAndColor; delete[] indices;
	//IMPORTANT: delete[] does not work on comma-separated pointers: https://cplusplus.com/forum/general/270369/#msg1164735
	//IMPORTANT: delete is the same: https://stackoverflow.com/questions/3037655/c-delete-syntax/3037675#3037675
}

inline void Diagnostics::drawGraphTimes_data() {
	for (int i = 0; i < graphTimes.size(); i++) {
		drawGraphTimes_data(graphTimes[i].name);
	}
}
