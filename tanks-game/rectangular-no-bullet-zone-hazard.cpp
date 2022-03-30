#include "rectangular-no-bullet-zone-hazard.h"
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

VertexArray* RectangularNoBulletZoneHazard::va;
VertexBuffer* RectangularNoBulletZoneHazard::vb;
IndexBuffer* RectangularNoBulletZoneHazard::ib;
VertexArray* RectangularNoBulletZoneHazard::extra_va;
VertexBuffer* RectangularNoBulletZoneHazard::extra_vb;
IndexBuffer* RectangularNoBulletZoneHazard::extra_ib;
bool RectangularNoBulletZoneHazard::initialized_GPU = false;

std::unordered_map<std::string, float> RectangularNoBulletZoneHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "random", .25f });
	return weights;
}

RectangularNoBulletZoneHazard::RectangularNoBulletZoneHazard(double xpos, double ypos, double width, double height) : RectHazard(HAZARD_TEAM) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;

	canAcceptPowers = false;

	modifiesTankCollision = true;
	modifiesBulletCollision = true;

	initializeGPU();
}

RectangularNoBulletZoneHazard::~RectangularNoBulletZoneHazard() {
	//uninitializeGPU();
}

bool RectangularNoBulletZoneHazard::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//background:
	float positions[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	vb = VertexBuffer::MakeVertexBuffer(positions, 4*2 * sizeof(float), RenderingHints::static_draw);
	VertexBufferLayout layout(2);
	va = VertexArray::MakeVertexArray(*vb, layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, 6);

	//red X:
	float extra_positions[] = {
		//forward slash:
		0.0,           0.0, //0
		0.0 + X_WIDTH, 0.0,
		0.0,           0.0 + X_WIDTH,

		1.0,           1.0, //3
		1.0 - X_WIDTH, 1.0,
		1.0,           1.0 - X_WIDTH,

		//backslash:
		0.0,           1.0, //6
		0.0,           1.0 - X_WIDTH,
		0.0 + X_WIDTH, 1.0,

		1.0,           0.0, //9
		1.0,           0.0 + X_WIDTH,
		1.0 - X_WIDTH, 0.0
	};
	unsigned int extra_indices[] = {
		//forward slash:
		0, 1, 2, //bottom left
		3, 4, 5, //top right
		1, 5, 4,
		4, 2, 1,

		//backslash:
		6, 7, 8, //top left
		9, 10, 11, //bottom right
		8, 7, 11,
		11, 10, 8
	};

	extra_vb = VertexBuffer::MakeVertexBuffer(extra_positions, 6*2*2 * sizeof(float), RenderingHints::static_draw);
	VertexBufferLayout extra_layout(2);
	extra_va = VertexArray::MakeVertexArray(*extra_vb, layout);

	extra_ib = IndexBuffer::MakeIndexBuffer(extra_indices, 8*3);

	initialized_GPU = true;
	return true;
}

bool RectangularNoBulletZoneHazard::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;

	initialized_GPU = false;
	return true;
}

RectHazard* RectangularNoBulletZoneHazard::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 4) {
			double* arr = (double*)(args.getDataPortion(0));
			double x = arr[0];
			double y = arr[1];
			double w = arr[2];
			double h = arr[3];
			return new RectangularNoBulletZoneHazard(x, y, w, h);
		}
	}
	return new RectangularNoBulletZoneHazard(0, 0, 0, 0);
}

bool RectangularNoBulletZoneHazard::reasonableLocation() const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::fullyCollided(this, WallManager::getWall(i))) {
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

void RectangularNoBulletZoneHazard::draw() const {
	ghostDraw(1.0f);
}

void RectangularNoBulletZoneHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularNoBulletZoneHazard::draw!" << std::endl;
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

void RectangularNoBulletZoneHazard::poseDraw() const {
	draw();
}

void RectangularNoBulletZoneHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularNoBulletZoneHazard::poseDraw!" << std::endl;
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

void RectangularNoBulletZoneHazard::ghostDraw(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	//background:
	ColorValueHolder color = GeneralizedNoBulletZone::getColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib, *shader);

	//red X:
	color = X_COLOR;
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, .75);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*extra_va, *extra_ib, *shader);
}

void RectangularNoBulletZoneHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularNoBulletZoneHazard::ghostDraw!" << std::endl;
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

RectHazard* RectangularNoBulletZoneHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData& args) {
	int attempts = 0;
	RectHazard* randomized = nullptr;
	double xpos, ypos, width, height;

	bool randomizeWH;
	int count = 0;
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);
	}
	if (count >= 2) {
		double* arr = (double*)(args.getDataPortion(0));
		width = arr[0];
		height = arr[1];
		randomizeWH = false;
	} else {
		randomizeWH = true;
	}

	do {
		if (randomizeWH) {
			width = RNG::randFunc() * (40 - 20) + 20; //TODO: where should these constants be?
			height = RNG::randFunc() * (50 - 20) + 20; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc() * (area_width - 2*width) + (x_start + width);
		ypos = RNG::randFunc() * (area_height - 2*height) + (y_start + height);
		RectHazard* testRectangularNoBulletZone = new RectangularNoBulletZoneHazard(xpos, ypos, width, height);
		if (testRectangularNoBulletZone->reasonableLocation()) {
			randomized = testRectangularNoBulletZone;
			break;
		} else {
			delete testRectangularNoBulletZone;
		}
		attempts++;
	} while (attempts < 64);

	return randomized;
}
