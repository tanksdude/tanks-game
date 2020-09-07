#include "circularlava.h"
#include "gamemanager.h"
#include "renderer.h"
#include "constants.h"
#include <math.h>
#include "mylib.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "collisionhandler.h"
#include "rng.h"

VertexArray* CircularLava::background_va;
VertexBuffer* CircularLava::background_vb;
IndexBuffer* CircularLava::background_ib;
VertexArray* CircularLava::bubble_va;
VertexBuffer* CircularLava::bubble_vb;
IndexBuffer* CircularLava::bubble_ib;
bool CircularLava::initialized_GPU = false;

CircularLava::CircularLava(double xpos, double ypos, double radius) {
	x = xpos;
	y = ypos;
	r = radius;
	gameID = GameManager::getNextID();
	teamID = HAZARD_TEAM;

	//tickCount = 0;
	tickCycle = 2400;
	bubbles.reserve(maxBubbles);
	bubbleChance = 1.0/400;
	
	canAcceptPowers = false;

	modifiesTankCollision = true;
	modifiesBulletCollision = true;

	initializeGPU();
}

CircularLava::~CircularLava() {
	//uninitializeGPU();
}

bool CircularLava::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//background:
	float background_positions[(Circle::numOfSides+1)*2];
	background_positions[0] = 0;
	background_positions[1] = 0;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		background_positions[i*2]   = cos((i-1) * 2*PI / Circle::numOfSides);
		background_positions[i*2+1] = sin((i-1) * 2*PI / Circle::numOfSides);
	}

	unsigned int background_indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		background_indices[i*3]   = 0;
		background_indices[i*3+1] = i+1;
		background_indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	background_vb = VertexBuffer::MakeVertexBuffer(background_positions, (Circle::numOfSides+1)*2 * sizeof(float), RenderingHints::static_draw);
	VertexBufferLayout background_layout(2);
	background_va = VertexArray::MakeVertexArray(*background_vb, background_layout);

	background_ib = IndexBuffer::MakeIndexBuffer(background_indices, Circle::numOfSides*3);

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

bool CircularLava::uninitializeGPU() {
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

CircleHazard* CircularLava::factory(int argc, std::string* argv) {
	if (argc >= 3) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double r = std::stod(argv[2]);
		return new CircularLava(x, y, r);
	}
	return new CircularLava(0, 0, 0);
}

void CircularLava::pushNewBubble(double radius) {
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

bool CircularLava::reasonableLocation() {
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

void CircularLava::draw() const {
	draw(x, y);
}

void CircularLava::draw(double xpos, double ypos) const {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(r, r, 0, xpos, ypos);
	
	//background:
	ColorValueHolder color = getBackgroundColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*background_va, *background_ib, *shader);

	//bubbles:
	if (bubbles.size() == 0) {
		return;
	}

	glLineWidth(2.0f);
	//first, sort by alpha: lowest to highest
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

	//second, draw the bubbles (this makes the bubbles less weird-looking when drawn over each other)
	for (int i = 0; i < sortedBubbles.size(); i++) {
		color = getBubbleColor(sortedBubbles[i]);
		MVPM = Renderer::GenerateMatrix(sortedBubbles[i]->getR(), sortedBubbles[i]->getR(), 0, sortedBubbles[i]->getX()*r + xpos, sortedBubbles[i]->getY()*r + ypos);

		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
		shader->setUniformMat4f("u_MVP", MVPM);

		Renderer::Draw(*bubble_va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);
	}
}

void CircularLava::poseDraw() const {
	//TODO
	return;
}

CircleHazard* CircularLava::randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv) {
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, radius;
	do {
		if (argc >= 1) {
			radius = std::stod(argv[0]);
		} else {
			radius = RNG::randFunc2() * (40 - 20) + 20; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc2() * (area_width - 2*radius) + (x_start + radius);
		ypos = RNG::randFunc2() * (area_height - 2*radius) + (y_start + radius);
		CircleHazard* testCircularLava = new CircularLava(xpos, ypos, radius);
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
