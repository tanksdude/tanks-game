#include "rectangular-lava-hazard.h"
#include "game-manager.h"
#include "renderer.h"
#include "constants.h"
#include <math.h>
#include "color-mixer.h"
#include "background-rect.h"
#include "mylib.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "collision-handler.h"
#include "rng.h"

VertexArray* RectangularLavaHazard::background_va;
VertexBuffer* RectangularLavaHazard::background_vb;
IndexBuffer* RectangularLavaHazard::background_ib;
VertexArray* RectangularLavaHazard::bubble_va;
VertexBuffer* RectangularLavaHazard::bubble_vb;
IndexBuffer* RectangularLavaHazard::bubble_ib;
bool RectangularLavaHazard::initialized_GPU = false;

std::unordered_map<std::string, float> RectangularLavaHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

RectangularLavaHazard::RectangularLavaHazard(double xpos, double ypos, double width, double height) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;
	gameID = GameManager::getNextID();
	teamID = HAZARD_TEAM;

	tickCount = 0;
	tickCycle = 2400;
	bubbles.reserve(maxBubbles);
	bubbleChance = 1.0/400;

	canAcceptPowers = false;

	modifiesTankCollision = true;
	modifiesBulletCollision = true;

	initializeGPU();
}

RectangularLavaHazard::~RectangularLavaHazard() {
	//uninitializeGPU();
}

bool RectangularLavaHazard::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//background:
	float background_positions[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};
	unsigned int background_indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	background_vb = VertexBuffer::MakeVertexBuffer(background_positions, 4*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout background_layout(2);
	background_va = VertexArray::MakeVertexArray(*background_vb, background_layout);

	background_ib = IndexBuffer::MakeIndexBuffer(background_indices, 6);

	//bubble:
	float bubble_positions[(Circle::numOfSides+1)*2];
	bubble_positions[0] = 0;
	bubble_positions[1] = 0;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		bubble_positions[i*2]   = cos((i-1) * 2*PI / Circle::numOfSides);
		bubble_positions[i*2+1] = sin((i-1) * 2*PI / Circle::numOfSides);
	}

	unsigned int bubble_indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		bubble_indices[i*3]   = 0;
		bubble_indices[i*3+1] = i+1;
		bubble_indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	bubble_vb = VertexBuffer::MakeVertexBuffer(bubble_positions, (Circle::numOfSides+1)*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout bubble_layout(2);
	bubble_va = VertexArray::MakeVertexArray(*bubble_vb, bubble_layout);

	bubble_ib = IndexBuffer::MakeIndexBuffer(bubble_indices, Circle::numOfSides*3);

	initialized_GPU = true;
	return true;
}

bool RectangularLavaHazard::uninitializeGPU() {
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

RectHazard* RectangularLavaHazard::factory(int argc, std::string* argv) {
	if (argc >= 4) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double w = std::stod(argv[2]);
		double h = std::stod(argv[3]);
		return new RectangularLavaHazard(x, y, w, h);
	}
	return new RectangularLavaHazard(0, 0, 0, 0);
}

void RectangularLavaHazard::pushNewBubble(double radius) {
	float x0, y0, x1, y1;
	int attempts = 0;

	x0 = RNG::randFunc() * (w - radius*2) + radius;
	y0 = RNG::randFunc() * (h - radius*2) + radius;
	do {
		x1 = RNG::randFunc() * (w - radius*2) + radius;
		y1 = RNG::randFunc() * (h - radius*2) + radius;
		attempts++;
	} while ((attempts < 8) && (abs(x0-x1) < w/16 || abs(y0-y1) < h/16)); //JS Tanks used w/8 and h/8

	if (attempts < 8) {
		double maxTick = floor(RNG::randFunc()*101) + 200;
		bubbles.push_back(new LavaBubble(radius, x0/w, y0/h, x1/w, y1/h, maxTick));
	}
}

bool RectangularLavaHazard::reasonableLocation() const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::partiallyCollided(this, WallManager::getWall(i))) {
			return false;
		}
	}

	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		if (CollisionHandler::partiallyCollided(this, HazardManager::getCircleHazard(i))) {
			return false;
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		RectHazard* rh = HazardManager::getRectHazard(i);
		if ((rh->getGameID() != this->getGameID()) && (rh->getName() != this->getName())) {
			if (CollisionHandler::partiallyCollided(this, rh)) {
				return false;
			}
		}
	}

	return validLocation();
}

void RectangularLavaHazard::draw() const {
	drawBackground(false);
	drawBubbles(false);
}

void RectangularLavaHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularLavaHazard::draw!" << std::endl;
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

void RectangularLavaHazard::poseDraw() const {
	drawBackground(true);
}

void RectangularLavaHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularLavaHazard::poseDraw!" << std::endl;
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

void RectangularLavaHazard::ghostDraw(float alpha) const {
	drawBackground(false, alpha);
	//no bubbles
}

void RectangularLavaHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularLavaHazard::ghostDraw!" << std::endl;
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

inline void RectangularLavaHazard::drawBackground(bool pose, float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	ColorValueHolder color = (pose ? getBackgroundColor_Pose() : getBackgroundColor());
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*background_va, *background_ib, *shader);
}

inline void RectangularLavaHazard::drawBubbles(bool pose, float alpha) const {
	if (bubbles.size() == 0) {
		return;
	}

	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	Renderer::SetLineWidth(2.0f);

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

		MVPM = Renderer::GenerateMatrix(sortedBubbles[i]->getR(), sortedBubbles[i]->getR(), 0, sortedBubbles[i]->getX()*w + x, sortedBubbles[i]->getY()*h + y);
		shader->setUniformMat4f("u_MVP", MVPM);

		Renderer::Draw(*bubble_va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

RectHazard* RectangularLavaHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv) {
	int attempts = 0;
	RectHazard* randomized = nullptr;
	double xpos, ypos, width, height;
	do {
		if (argc >= 2) {
			width = std::stod(argv[0]);
			height = std::stod(argv[1]);
		} else {
			width = RNG::randFunc2() * (120 - 30) + 30; //TODO: where should these constants be?
			height = RNG::randFunc2() * (80 - 20) + 20; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc2() * (area_width - 2*width) + (x_start + width);
		ypos = RNG::randFunc2() * (area_height - 2*height) + (y_start + height);
		RectHazard* testRectangularLava = new RectangularLavaHazard(xpos, ypos, width, height);
		if (testRectangularLava->reasonableLocation()) {
			randomized = testRectangularLava;
			break;
		} else {
			delete testRectangularLava;
		}
		attempts++;
	} while (attempts < 64);

	return randomized;
}
