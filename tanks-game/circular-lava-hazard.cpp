#include "circular-lava-hazard.h"
#include "renderer.h"
#include "constants.h"
#include <cmath>
#include "color-mixer.h"
#include "background-rect.h"
#include "mylib.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "collision-handler.h"
#include "rng.h"

VertexArray* CircularLavaHazard::background_va;
VertexBuffer* CircularLavaHazard::background_vb;
IndexBuffer* CircularLavaHazard::background_ib;
VertexArray* CircularLavaHazard::bubble_va;
VertexBuffer* CircularLavaHazard::bubble_vb;
IndexBuffer* CircularLavaHazard::bubble_ib;
bool CircularLavaHazard::initialized_GPU = false;

std::unordered_map<std::string, float> CircularLavaHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

CircularLavaHazard::CircularLavaHazard(double xpos, double ypos, double radius) : CircleHazard(HAZARD_TEAM) {
	x = xpos;
	y = ypos;
	r = radius;

	tickCount = 0;
	tickCycle = 2400;
	bubbles.reserve(maxBubbles);
	bubbleChance = 1.0/400;

	canAcceptPowers = false;

	modifiesTankCollision = true;
	modifiesBulletCollision = true;

	initializeGPU();
}

CircularLavaHazard::~CircularLavaHazard() {
	//uninitializeGPU();
}

bool CircularLavaHazard::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//background:
	float background_positions[(Circle::numOfSides+1)*(2+4)];
	background_positions[0] = 0;
	background_positions[1] = 0;
	background_positions[2] = 1.0f;
	background_positions[3] = 0.5f;
	background_positions[4] = 0.0f;
	background_positions[5] = 1.0f;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		background_positions[i*6]   = cos((i-1) * 2*PI / Circle::numOfSides);
		background_positions[i*6+1] = sin((i-1) * 2*PI / Circle::numOfSides);
		background_positions[i*6+2] = 1.0f;
		background_positions[i*6+3] = 0.5f;
		background_positions[i*6+4] = 0.0f;
		background_positions[i*6+5] = 1.0f;
	}

	unsigned int background_indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		background_indices[i*3]   = 0;
		background_indices[i*3+1] = i+1;
		background_indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	background_vb = VertexBuffer::MakeVertexBuffer(background_positions, (Circle::numOfSides+1)*(2+4) * sizeof(float), RenderingHints::static_draw);
	VertexBufferLayout background_layout = {
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	background_vb->SetLayout(background_layout);

	background_ib = IndexBuffer::MakeIndexBuffer(background_indices, Circle::numOfSides*3);

	background_va = VertexArray::MakeVertexArray();
	background_va->AddVertexBuffer(background_vb);
	background_va->SetIndexBuffer(background_ib);

	//bubble:
	float bubble_positions[(Circle::numOfSides+1)*(2+4)];
	bubble_positions[0] = 0;
	bubble_positions[1] = 0;
	bubble_positions[2] = 1.0f;
	bubble_positions[3] = 1.0f;
	bubble_positions[4] = 1.0f;
	bubble_positions[5] = 1.0f;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		bubble_positions[i*6]   = cos((i-1) * 2*PI / Circle::numOfSides);
		bubble_positions[i*6+1] = sin((i-1) * 2*PI / Circle::numOfSides);
		bubble_positions[i*6+2] = 1.0f;
		bubble_positions[i*6+3] = 1.0f;
		bubble_positions[i*6+4] = 1.0f;
		bubble_positions[i*6+5] = 1.0f;
	}

	unsigned int bubble_indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		bubble_indices[i*3]   = 0;
		bubble_indices[i*3+1] = i+1;
		bubble_indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	bubble_vb = VertexBuffer::MakeVertexBuffer(bubble_positions, (Circle::numOfSides+1)*(2+4) * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout bubble_layout = {
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	bubble_vb->SetLayout(bubble_layout);

	bubble_ib = IndexBuffer::MakeIndexBuffer(bubble_indices, Circle::numOfSides*3);

	bubble_va = VertexArray::MakeVertexArray();
	bubble_va->AddVertexBuffer(bubble_vb);
	bubble_va->SetIndexBuffer(bubble_ib);

	initialized_GPU = true;
	return true;
}

bool CircularLavaHazard::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;
	delete bubble_va;
	delete bubble_vb;
	delete bubble_ib;

	initialized_GPU = false;
	return true;
}

CircleHazard* CircularLavaHazard::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 3) {
			double* arr = (double*)(args.getDataPortion(0));
			double x = arr[0];
			double y = arr[1];
			double r = arr[2];
			return new CircularLavaHazard(x, y, r);
		}
	}
	return new CircularLavaHazard(0, 0, 0);
}

void CircularLavaHazard::pushNewBubble(double radius) {
	float x0, y0, x1, y1;
	int attempts = 0;

	float r0, a0, r1, a1;
	r0 = RNG::randFunc() * (r - radius);
	a0 = RNG::randFunc() * (2*PI);
	x0 = r0 * cos(a0);
	y0 = r0 * sin(a0);
	do {
		r1 = RNG::randFunc() * (r - radius);
		a1 = RNG::randFunc() * (2 * PI);
		x1 = r1 * cos(a1);
		y1 = r1 * sin(a1);
		attempts++;
	} while ((attempts < 8) && (abs(x0-x1) < r/16 || abs(y0-y1) < r/16));

	if (attempts < 8) {
		double maxTick = floor(RNG::randFunc()*101) + 200;
		bubbles.push_back(new LavaBubble(radius, x0/r, y0/r, x1/r, y1/r, maxTick));
	}
}

bool CircularLavaHazard::reasonableLocation() const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::partiallyCollided(this, WallManager::getWall(i))) {
			return false;
		}
	}

	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		CircleHazard* ch = HazardManager::getCircleHazard(i);
		if ((ch->getGameID() != this->getGameID()) && (ch->getName() != this->getName())) {
			//TODO: does this care if it's colliding with another version of itself?
			if (CollisionHandler::partiallyCollided(this, ch)) {
				return false;
			}
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		if (CollisionHandler::partiallyCollided(this, HazardManager::getRectHazard(i))) {
			return false;
		}
	}

	return validLocation();
}

void CircularLavaHazard::draw() const {
	drawBackground(false);
	drawBubbles(false);
}

void CircularLavaHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularLavaHazard::draw!" << std::endl;
		case DrawingLayers::under:
			drawBackground(false);
			break;

		case DrawingLayers::normal:
			drawBubbles(false);
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void CircularLavaHazard::poseDraw() const {
	drawBackground(true);
}

void CircularLavaHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularLavaHazard::poseDraw!" << std::endl;
		case DrawingLayers::under:
			drawBackground(true);
			break;

		case DrawingLayers::normal:
			//drawBubbles(true);
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void CircularLavaHazard::ghostDraw(float alpha) const {
	drawBackground(false, alpha);
	//no bubbles
}

void CircularLavaHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularLavaHazard::ghostDraw!" << std::endl;
		case DrawingLayers::under:
			drawBackground(false, alpha);
			break;

		case DrawingLayers::normal:
			//drawBubbles(false, alpha);
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

inline void CircularLavaHazard::drawBackground(bool pose, float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	/*
	ColorValueHolder color = (pose ? getBackgroundColor_Pose() : getBackgroundColor());
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*background_va, *background_ib, *shader);
	*/

	ColorValueHolder color = (pose ? getBackgroundColor_Pose() : getBackgroundColor());
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

	float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
	coordsAndColor[0] = x;
	coordsAndColor[1] = y;
	coordsAndColor[2] = color.getRf();
	coordsAndColor[3] = color.getGf();
	coordsAndColor[4] = color.getBf();
	coordsAndColor[5] = color.getAf();
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		coordsAndColor[i*6]   = x + r * cos((i-1) * 2*PI / Circle::numOfSides);
		coordsAndColor[i*6+1] = y + r * sin((i-1) * 2*PI / Circle::numOfSides);
		coordsAndColor[i*6+2] = color.getRf();
		coordsAndColor[i*6+3] = color.getGf();
		coordsAndColor[i*6+4] = color.getBf();
		coordsAndColor[i*6+5] = color.getAf();
	}

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = 0;
		indices[i*3+1] = i+1;
		indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides+1)*(2+4), indices, Circle::numOfSides*3);
}

inline void CircularLavaHazard::drawBubbles(bool pose, float alpha) const {
	if (bubbles.size() == 0) {
		return;
	}

	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	glLineWidth(2.0f);

	//first, sort by alpha: lowest to highest (this makes the bubbles less weird-looking when drawn over each other)
	std::vector<LavaBubble*> sortedBubbles;
	sortedBubbles.reserve(bubbles.size());
	for (int i = 0; i < bubbles.size(); i++) {
		//insertion sort because laziness
		sortedBubbles.push_back(bubbles[i]);
		for (int j = sortedBubbles.size()-1; j >= 1; j--) {
			if (sortedBubbles[j]->getAlpha() < sortedBubbles[j-1]->getAlpha()) {
				std::swap(sortedBubbles[j], sortedBubbles[j-1]);
			} else {
				break;
			}
		}
	}

	//second, draw the bubbles
	for (int i = 0; i < sortedBubbles.size(); i++) {
		ColorValueHolder color = (pose ? getBubbleColor_Pose(sortedBubbles[i]) : getBubbleColor(sortedBubbles[i]));
		color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

		modelMatrix = Renderer::GenerateModelMatrix(sortedBubbles[i]->getR(), sortedBubbles[i]->getR(), 0, sortedBubbles[i]->getX()*r + x, sortedBubbles[i]->getY()*r + y);
		shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

		Renderer::Draw(*bubble_va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

CircleHazard* CircularLavaHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData& args) {
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, radius;

	bool randomizeR;
	int count = 0;
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);
	}
	if (count >= 1) {
		double* arr = (double*)(args.getDataPortion(0));
		radius = arr[0];
		randomizeR = false;
	} else {
		randomizeR = true;
	}

	do {
		if (randomizeR) {
			radius = RNG::randFunc() * (40 - 20) + 20; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc() * (area_width - 2*radius) + (x_start + radius);
		ypos = RNG::randFunc() * (area_height - 2*radius) + (y_start + radius);
		CircleHazard* testCircularLava = new CircularLavaHazard(xpos, ypos, radius);
		if (testCircularLava->reasonableLocation()) {
			randomized = testCircularLava;
			break;
		} else {
			delete testCircularLava;
		}
		attempts++;
	} while (attempts < 64);

	return randomized;
}
