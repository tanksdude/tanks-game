#include "frame-time-graph.h"

#include "constants.h"
#include <algorithm> //std::min, std::max
#include <iostream>

#include "renderer.h"
#include "simple-vector-2d.h"

std::vector<FrameTimeGraph::GraphData> FrameTimeGraph::graphTimes;
std::unordered_map<std::string, int> FrameTimeGraph::graphNameToIndex;
int FrameTimeGraph::maxGraphTimes = 200;
float FrameTimeGraph::graphLength = GAME_WIDTH/4;
float FrameTimeGraph::graphHeight = GAME_HEIGHT/4;
float FrameTimeGraph::graphXOffset = 0;
float FrameTimeGraph::graphYOffset = 0;

FrameTimeGraph::GraphData::GraphData(std::string n, const ColorValueHolder& c) {
	this->name = n;
	this->color = c;
}

void FrameTimeGraph::setGraphYOffset(float y) {
	graphYOffset = y;
}

void FrameTimeGraph::Initialize() {
	//nothing
}

void FrameTimeGraph::declareGraph(std::string name, const ColorValueHolder& color) {
	graphNameToIndex.insert({ name, graphTimes.size() });
	graphTimes.push_back(GraphData(name, color));
}

void FrameTimeGraph::pushGraphTime(std::string name, time_float time) {
	std::vector<time_float>& currentGraph = graphTimes[graphNameToIndex[name]].data;

	currentGraph.push_back(time);
	if (currentGraph.size() > maxGraphTimes) {
		const unsigned int count = currentGraph.size() - maxGraphTimes;
		currentGraph.erase(currentGraph.begin(), currentGraph.begin() + count);
	}

	if (maxGraphTimes <= 200) {
		graphLength = GAME_WIDTH/4;
	} else if (maxGraphTimes >= 2000) {
		graphLength = GAME_WIDTH/2;
	} else {
		graphLength = (maxGraphTimes-200)/2000.0f * float(GAME_WIDTH/4) + float(GAME_WIDTH/4);
	}
}

void FrameTimeGraph::clearGraph(std::string name) {
	graphTimes[graphNameToIndex[name]].data.clear();
}

void FrameTimeGraph::clearGraphs() {
	for (int i = 0; i < graphTimes.size(); i++) {
		graphTimes[i].data.clear();
	}
}

void FrameTimeGraph::pushGraphSumTime(std::string name) {
	time_float sum = 0;
	for (int i = 0; i < graphTimes.size(); i++) {
		if ((graphTimes[i].name == name) || (graphTimes[i].data.size() == 0)) {
			continue;
		}
		sum += graphTimes[i].data[graphTimes[i].data.size()-1];
		//technically not right; the proper implementation is to check everything that got updated and sum those
	}

	pushGraphTime(name, sum);
}

void FrameTimeGraph::drawGraphTimes() {
	drawGraphTimes_graph();
	drawGraphTimes_data();
}

void FrameTimeGraph::drawGraphTimes(std::string name) {
	drawGraphTimes_graph();
	drawGraphTimes_data(name);
}

void FrameTimeGraph::drawGraphTimes_graph() {
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

void FrameTimeGraph::drawGraphTimes_data(std::string name) {
	const std::vector<time_float>& graphData = graphTimes[graphNameToIndex[name]].data;
	if (graphData.size() < 2) [[unlikely]] {
		return;
	}

	ColorValueHolder color = graphTimes[graphNameToIndex[name]].color;
	const float lineWidth = 1.0f;

	std::vector<float> positions; positions.reserve(graphData.size()*2);
	for (int i = 0; i < graphData.size(); i++) {
		positions.push_back(graphLength * (float(i)/float(maxGraphTimes-1)));
		positions.push_back(graphHeight * (float(graphData[i]) * (1.0f / 10))); //==1 means it took a full frame (10ms)
	}

	float* coordsAndColor = new float[(graphData.size()-1)*4*(2+4)];
	unsigned int* indices = new unsigned int[(graphData.size()-1)*6];

	for (int j = 0; j < graphData.size()-1; j++) {
		const int startVertex = (j*4) * 6;
		const int startIndex = j*6;

		SimpleVector2D dist = SimpleVector2D(positions[(j+1)*2] - positions[j*2], positions[(j+1)*2+1] - positions[j*2+1]);
		SimpleVector2D distCW = SimpleVector2D(dist.getAngle() - float(PI/2), lineWidth, true);

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

void FrameTimeGraph::drawGraphTimes_data() {
	for (int i = 0; i < graphTimes.size(); i++) {
		drawGraphTimes_data(graphTimes[i].name);
	}
}
