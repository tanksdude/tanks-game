#include "diagnostics.h"
#include <iostream>
#include "renderer.h"
#include "constants.h"
#include <algorithm> //std::min, std::max

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

VertexArray* Diagnostics::graph_va;
VertexBuffer* Diagnostics::graph_vb;
//IndexBuffer* Diagnostics::graph_ib;
VertexArray* Diagnostics::data_va;
VertexBuffer* Diagnostics::data_vb;
int Diagnostics::data_vb_length;
bool Diagnostics::initialized_GPU = false;

Diagnostics::GraphData::GraphData(std::string n, ColorValueHolder c) {
	this->name = n;
	this->color = c;
}

void Diagnostics::setGraphYOffset(double y) {
	graphYOffset = y;
}

void Diagnostics::Initialize() {
	initializeGPU();
	//streamDataGPU(); //unnecessary?
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

bool Diagnostics::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//x-axis and y-axis
	float graph_positions[] = {
		0, 0,
		1, 0,
		1, 1, //maybe an outline is wanted instead of just axes
		0, 1
	};
	/*
	unsigned int graph_indices[] = {
		0, 1,
		0, 2
	};
	*/

	graph_vb = VertexBuffer::MakeVertexBuffer(graph_positions, 4*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout graph_layout(2);
	graph_va = VertexArray::MakeVertexArray(*graph_vb, graph_layout);

	//unused:
	//graph_ib = IndexBuffer::MakeIndexBuffer(graph_indices, 4);

	//eh
	float* data_positions = new float[maxGraphTimes*2];
	/*
	for (int i = 0; i < maxGraphTimes; i++) {
		data_positions[i*2]   = float(i)/float(maxGraphTimes-1);
		data_positions[i*2+1] = .5;
	}
	std::vector<float> data_positions; data_positions.reserve(maxGraphTimes*2);
	for (int i = 0; i < maxGraphTimes; i++) {
		data_positions.push_back(float(i)/float(maxGraphTimes-1));
		data_positions.push_back(.5);
	}
	*/
	data_vb_length = maxGraphTimes;

	data_vb = VertexBuffer::MakeVertexBuffer(data_positions, maxGraphTimes*2 * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout data_layout(2);
	data_va = VertexArray::MakeVertexArray(*data_vb, data_layout);

	delete[] data_positions;
	initialized_GPU = true;
	return true;
}

bool Diagnostics::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete graph_vb;
	delete graph_va;
	//delete graph_ib;
	delete data_vb;
	delete data_va;

	initialized_GPU = false;
	return true;
}

void Diagnostics::streamDataGPU(const std::vector<long double>& graphData) {
	//float* positions = new float[graphData.size()*2];
	std::vector<float> positions; positions.reserve(graphData.size()*2);
	for (int i = 0; i < graphData.size(); i++) {
		//positions[i*2]   = float(i)/float(maxGraphTimes);
		positions.push_back(float(i)/float(maxGraphTimes-1));
		//positions[i*2+1] = graphData[i] * (1.0 / 10);
		positions.push_back(graphData[i] * (1.0 / 10)); //==1 means it took a full frame (10ms)
		//positions.push_back(std::min(graphData[i] * (1.0 / 10), (long double)2));
	}

	if (graphData.size() > data_vb_length) {
		delete data_vb;
		delete data_va;

		data_vb = VertexBuffer::MakeVertexBuffer(positions.data(), graphData.size()*2 * sizeof(float), RenderingHints::stream_draw);
		VertexBufferLayout layout(2);
		data_va = VertexArray::MakeVertexArray(*data_vb, layout);
		data_vb_length = graphData.size();
	} else {
		data_vb->modifyData(positions.data(), graphData.size()*2 * sizeof(float));
	}

	//delete[] positions;
}

void Diagnostics::declareGraph(std::string name, ColorValueHolder color) {
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

void Diagnostics::drawGraphTimes() {
	drawGraphTimes_graph();
	drawGraphTimes_data();
}

void Diagnostics::drawGraphTimes(std::string name) {
	drawGraphTimes_graph();
	drawGraphTimes_data(name);
}

void Diagnostics::drawGraphTimes_graph() {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	glLineWidth(4.0f); //TODO: change?

	ColorValueHolder color = ColorValueHolder(0.75f, 0.75f, 0.75f);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(graphLength, graphHeight, 0, graphXOffset, graphYOffset);
	shader->setUniformMat4f("u_MVP", MVPM);

	//Renderer::Draw(*graph_va, *shader, GL_LINE_LOOP, 0, 4);
	Renderer::Draw(*graph_va, *shader, GL_LINE_STRIP, 1, 3);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Diagnostics::drawGraphTimes_data(std::string name) {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	glLineWidth(4.0f); //TODO: change?

	ColorValueHolder color = graphTimes[graphNameToIndex[name]].color;
	//ColorValueHolder color = ColorValueHolder(1.0f, 1.0f, 1.0f);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(graphLength, graphHeight, 0, graphXOffset, graphYOffset);
	shader->setUniformMat4f("u_MVP", MVPM);

	streamDataGPU(graphTimes[graphNameToIndex[name]].data);
	Renderer::Draw(*data_va, *shader, GL_LINE_STRIP, 0, graphTimes[graphNameToIndex[name]].data.size());

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Diagnostics::drawGraphTimes_data() {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	glLineWidth(4.0f); //TODO: change?

	MVPM = Renderer::GenerateMatrix(graphLength, graphHeight, 0, graphXOffset, graphYOffset);
	shader->setUniformMat4f("u_MVP", MVPM);

	for (int i = 0; i < graphTimes.size(); i++) {
		ColorValueHolder color = graphTimes[i].color;
		//ColorValueHolder color = ColorValueHolder(1.0f, 1.0f, 1.0f);
		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

		streamDataGPU(graphTimes[i].data);
		Renderer::Draw(*data_va, *shader, GL_LINE_STRIP, 0, graphTimes[i].data.size());
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
