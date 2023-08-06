#include "circular-no-bullet-zone-hazard.h"
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

VertexArray* CircularNoBulletZoneHazard::va;
VertexBuffer* CircularNoBulletZoneHazard::vb;
IndexBuffer* CircularNoBulletZoneHazard::ib;
//VertexArray* CircularNoBulletZoneHazard::extra_va;
//VertexBuffer* CircularNoBulletZoneHazard::extra_vb;
IndexBuffer* CircularNoBulletZoneHazard::extra_ib;
bool CircularNoBulletZoneHazard::initialized_GPU = false;

std::unordered_map<std::string, float> CircularNoBulletZoneHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "random", .25f });
	return weights;
}

CircularNoBulletZoneHazard::CircularNoBulletZoneHazard(double xpos, double ypos, double radius) : CircleHazard(HAZARD_TEAM) {
	x = xpos;
	y = ypos;
	r = radius;

	canAcceptPowers = false;

	modifiesTankCollision = true;
	modifiesBulletCollision = true;

	initializeGPU();
}

CircularNoBulletZoneHazard::~CircularNoBulletZoneHazard() {
	//uninitializeGPU();
}

bool CircularNoBulletZoneHazard::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//background:
	float positions[(Circle::numOfSides+1)*2];
	positions[0] = 0;
	positions[1] = 0;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		positions[i*2]   = cos((i-1) * 2*PI / Circle::numOfSides);
		positions[i*2+1] = sin((i-1) * 2*PI / Circle::numOfSides);
	}

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = 0;
		indices[i*3+1] = i+1;
		indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	vb = VertexBuffer::MakeVertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float), RenderingHints::static_draw);
	VertexBufferLayout layout(2);
	va = VertexArray::MakeVertexArray(*vb, layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	//red X:
	//compared to the rectangular version, the width and height will basically be 1/4 of the circumference
	//num vertices for one half of the X on one edge: Circle::numOfSides/4 * X_WIDTH
	//const int num_portion_indices = ceil(Circle::numOfSides/4 * X_WIDTH); //2
	const int num_portion_indices = floor(Circle::numOfSides/4 * X_WIDTH); //closer to the rectangular version
	const int num_extra_indicies = 8*num_portion_indices; //ceil: 16; floor: 8
	//unsigned int* extra_indices = new unsigned int[num_extra_indicies];
	unsigned int* real_extra_indices = new unsigned int[num_extra_indicies*3 + 3*4];

	for (int k = 0; k < 4; k++) {
		for (int i = -1*num_portion_indices; i < num_portion_indices; i++) {
			int val = (k*Circle::numOfSides/4 + Circle::numOfSides/8) + i + 1;
			int insert_index = k*(2*num_portion_indices) + (i + num_portion_indices);
			real_extra_indices[insert_index*3]   = 0;
			real_extra_indices[insert_index*3+1] = val;
			real_extra_indices[insert_index*3+2] = val+1;
		}
	}

	//full triangles like the rectangular version
	/*
	real_extra_indices[num_extra_indicies*3 +  0] = (0*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 +  1] = (0*Circle::numOfSides/4 + Circle::numOfSides/8) + num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 +  2] = (2*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;

	real_extra_indices[num_extra_indicies*3 +  3] = (2*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 +  4] = (2*Circle::numOfSides/4 + Circle::numOfSides/8) + num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 +  5] = (0*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;

	real_extra_indices[num_extra_indicies*3 +  6] = (1*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 +  7] = (1*Circle::numOfSides/4 + Circle::numOfSides/8) + num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 +  8] = (3*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;

	real_extra_indices[num_extra_indicies*3 +  9] = (3*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 + 10] = (3*Circle::numOfSides/4 + Circle::numOfSides/8) + num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 + 11] = (1*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;
	*/

	//the X lines don't draw over the previously-drawn arcs
	real_extra_indices[num_extra_indicies*3 +  0] = 0;
	real_extra_indices[num_extra_indicies*3 +  1] = (2*Circle::numOfSides/4 + Circle::numOfSides/8) + num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 +  2] = (0*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;

	real_extra_indices[num_extra_indicies*3 +  3] = 0;
	real_extra_indices[num_extra_indicies*3 +  4] = (0*Circle::numOfSides/4 + Circle::numOfSides/8) + num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 +  5] = (2*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;

	real_extra_indices[num_extra_indicies*3 +  6] = 0;
	real_extra_indices[num_extra_indicies*3 +  7] = (3*Circle::numOfSides/4 + Circle::numOfSides/8) + num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 +  8] = (1*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;

	real_extra_indices[num_extra_indicies*3 +  9] = 0;
	real_extra_indices[num_extra_indicies*3 + 10] = (1*Circle::numOfSides/4 + Circle::numOfSides/8) + num_portion_indices + 1;
	real_extra_indices[num_extra_indicies*3 + 11] = (3*Circle::numOfSides/4 + Circle::numOfSides/8) - num_portion_indices + 1;

	extra_ib = IndexBuffer::MakeIndexBuffer(real_extra_indices, num_extra_indicies*3 + 3*4);
	delete[] real_extra_indices;

	initialized_GPU = true;
	return true;
}

bool CircularNoBulletZoneHazard::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;

	initialized_GPU = false;
	return true;
}

CircleHazard* CircularNoBulletZoneHazard::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 3) {
			double* arr = (double*)(args.getDataPortion(0));
			double x = arr[0];
			double y = arr[1];
			double r = arr[2];
			return new CircularNoBulletZoneHazard(x, y, r);
		}
	}
	return new CircularNoBulletZoneHazard(0, 0, 0);
}

bool CircularNoBulletZoneHazard::reasonableLocation() const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::fullyCollided(this, WallManager::getWall(i))) {
			return false;
		}
	}

	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		CircleHazard* ch = HazardManager::getCircleHazard(i);
		if ((ch->getGameID() != this->getGameID()) && (ch->getName() != this->getName())) {
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

void CircularNoBulletZoneHazard::draw() const {
	ghostDraw(1.0f);
}

void CircularNoBulletZoneHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularNoBulletZoneHazard::draw!" << std::endl;
		case DrawingLayers::under:
			draw();
			break;

		case DrawingLayers::normal:
			//nothing
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//nothing
			break;
	}
}

void CircularNoBulletZoneHazard::poseDraw() const {
	draw();
}

void CircularNoBulletZoneHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularNoBulletZoneHazard::poseDraw!" << std::endl;
		case DrawingLayers::under:
			poseDraw();
			break;

		case DrawingLayers::normal:
			//nothing
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//nothing
			break;
	}
}

void CircularNoBulletZoneHazard::ghostDraw(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	//background:
	ColorValueHolder color = GeneralizedNoBulletZone::getColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*va, *ib, *shader);

	//red X:
	color = X_COLOR;
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, .75);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*va, *extra_ib, *shader);
}

void CircularNoBulletZoneHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularNoBulletZoneHazard::ghostDraw!" << std::endl;
		case DrawingLayers::under:
			ghostDraw(alpha);
			break;

		case DrawingLayers::normal:
			//nothing
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//nothing
			break;
	}
}

CircleHazard* CircularNoBulletZoneHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData& args) {
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
			radius = RNG::randFunc() * (20 - 10) + 10; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc() * (area_width - 2*radius) + (x_start + radius);
		ypos = RNG::randFunc() * (area_height - 2*radius) + (y_start + radius);
		CircleHazard* testCircularNoBulletZone = new CircularNoBulletZoneHazard(xpos, ypos, radius);
		if (testCircularNoBulletZone->reasonableLocation()) {
			randomized = testCircularNoBulletZone;
			break;
		} else {
			delete testCircularNoBulletZone;
		}
		attempts++;
	} while (attempts < 64);

	return randomized;
}
