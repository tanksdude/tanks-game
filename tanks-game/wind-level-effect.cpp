#include "wind-level-effect.h"
#include "constants.h"
#include <cmath>
#include "rng.h"
#include "tank-manager.h"
#include "bullet-manager.h"
#include "color-mixer.h"
#include "background-rect.h"
#include "renderer.h"

VertexArray* WindLevelEffect::va;
VertexBuffer* WindLevelEffect::vb;
IndexBuffer* WindLevelEffect::ib;
VertexArray* WindLevelEffect::va_extra;
VertexBuffer* WindLevelEffect::vb_extra;
IndexBuffer* WindLevelEffect::ib_extra;
bool WindLevelEffect::initialized_GPU = false;

std::unordered_map<std::string, float> WindLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

double WindLevelEffect::getWindStrengthMultiplier() const {
	switch (currentState) {
		case 0: return 0;
		case 1: return tickCount / (tickCycle * stateMultiplier[1]);
		case 2: return 1;
		case 3: return 1 - tickCount / (tickCycle * stateMultiplier[3]);
		default: return 0;
	}
}

void WindLevelEffect::apply() {
	//nothing
}

void WindLevelEffect::tick(const Level* parent) {
	tickCount++;
	bool changeWind = false;
	//TODO: account for states to be skipped entirely, better
	while (tickCount >= tickCycle * stateMultiplier[currentState]) {
		if (tickCycle * stateMultiplier[currentState] <= 0) {
			tickCount = 0;
			++currentState %= maxState;
			if (currentState == 1) {
				changeWind = true;
			}
			break;
		} else {
			tickCount -= tickCycle * stateMultiplier[currentState];
			++currentState %= maxState;
			if (currentState == 1) {
				changeWind = true;
			}
		}
	}
	if (changeWind) {
		//pushDirection = SimpleVector2D(RNG::randFunc() * 2*PI, RNG::randFunc(), true); //JS, I think (~90% sure)
		pushDirection = SimpleVector2D(RNG::randFunc() * 2*PI, RNG::randFunc()*.875 + .125, true);
	}
	//std::cout << currentState << " " << getWindStrengthMultiplier() << std::endl;
}

void WindLevelEffect::doEffects(Level* parent) {
	if (getWindStrengthMultiplier() > 0) {
		for (int i = 0; i < TankManager::getNumTanks(); i++) {
			Tank* t = TankManager::getTank(i);
			t->x += pushDirection.getXComp() * getWindStrengthMultiplier();
			t->y += pushDirection.getYComp() * getWindStrengthMultiplier();
		}
		for (int i = 0; i < BulletManager::getNumBullets(); i++) {
			Bullet* b = BulletManager::getBullet(i);
			b->x += pushDirection.getXComp() * getWindStrengthMultiplier();
			b->y += pushDirection.getYComp() * getWindStrengthMultiplier();
		}
	}
}

void WindLevelEffect::draw() const {
	//length = 16 * pushDirection.getMagnitude()
	//7 x 5 arrows
	//color: ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(0.0f, 0.0f, 0.0f), .5);

	if (getWindStrengthMultiplier() <= 0) {
		return;
	}

	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	ColorValueHolder color = ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(0.0f, 0.0f, 0.0f));
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	const double length = 16 * pushDirection.getMagnitude() * getWindStrengthMultiplier(); //normal
	//const double length = 16 * sqrt(pushDirection.getMagnitude() * getWindStrengthMultiplier()); //ez way to make small values more visible
	//sqrt is not a big deal to compute: https://stackoverflow.com/questions/41582376/fast-approximation-of-square-rootx-with-known-0-x-1
	//const double length = 16 * sqrt(1 - pow(pushDirection.getMagnitude()*getWindStrengthMultiplier()-1, 2)); //(circle) //too big
	const double x_offset = 64; //JS x offset: (528-112)/7 (it's the distance between the main walls) = 59.42857
	const double y_offset = 64; //JS y offset: (320)/5 = 64
	for (int i = -3; i <= 3; i++) {
		for (int j = -2; j <= 2; j++) {
			MVPM = Renderer::GenerateMatrix(length, length, pushDirection.getAngle(), GAME_WIDTH/2 + i*x_offset, GAME_HEIGHT/2 + j*y_offset);
			shader->setUniformMat4f("u_MVP", MVPM);

			if (i == 0 && j == 0) {
				Renderer::Draw(*va_extra, *ib_extra, *shader);
			} else {
				Renderer::Draw(*va, *ib, *shader);
			}
		}
	}
}

void WindLevelEffect::draw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for WindLevelEffect::draw!" << std::endl;
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
			//later
			break;
	}
}

void WindLevelEffect::poseDraw() const {
	ghostDraw(1.0f);
}

void WindLevelEffect::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for WindLevelEffect::poseDraw!" << std::endl;
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
			//later
			break;
	}
}

void WindLevelEffect::ghostDraw(float alpha) const {
	//from draw() (maybe copy-pasting isn't a good idea)
	//7 x 5 arrows
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	ColorValueHolder color = ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(0.0f, 0.0f, 0.0f));
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	const double length = 16 * pushDirection.getMagnitude();
	const double x_offset = 64;
	const double y_offset = 64;
	for (int i = -3; i <= 3; i++) {
		for (int j = -2; j <= 2; j++) {
			if (i == -j) {
				continue;
			}

			double angle;
			if (i < -j) {
				//bottom left
				angle = -3*PI/4;
			} else {
				//top right
				angle = PI/4;
			}

			MVPM = Renderer::GenerateMatrix(length, length, angle, GAME_WIDTH/2 + i*x_offset, GAME_HEIGHT/2 + j*y_offset);
			shader->setUniformMat4f("u_MVP", MVPM);

			Renderer::Draw(*va, *ib, *shader);
		}
	}
}

void WindLevelEffect::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for WindLevelEffect::ghostDraw!" << std::endl;
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
			//later
			break;
	}
}

bool WindLevelEffect::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//I think an arrow was originally intended but complex to implement, so a spike was used

	//spike:
	float positions[] = {
		-1.0f, -0.125f,
		-1.0f,  0.125f,
		 1.0f,  0.0f
	};
	//arrow:
	float positions_extra[] = {
		-1.0f, -0.125f, //bottom left  //0
		-1.0f,  0.125f, //top left     //1
		 0.5f,  0.125f, //top right    //2
		 0.5f,  0.5f,                  //3
		 1.0f,  0.0f,   //arrow tip    //4
		 0.5f, -0.5f,                  //5
		 0.5f, -0.125f  //bottom right //6
	};

	unsigned int indices[] = {
		0, 1, 2
	};
	unsigned int indices_extra[] = {
		0, 1, 2,
		2, 6, 0,
		3, 4, 5
	};

	vb = VertexBuffer::MakeVertexBuffer(positions, sizeof(positions), RenderingHints::dynamic_draw);
	VertexBufferLayout layout(2);
	va = VertexArray::MakeVertexArray(*vb, layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, sizeof(indices)/sizeof(unsigned int));

	vb_extra = VertexBuffer::MakeVertexBuffer(positions_extra, sizeof(positions_extra), RenderingHints::dynamic_draw);
	VertexBufferLayout layout_extra(2);
	va_extra = VertexArray::MakeVertexArray(*vb_extra, layout_extra);

	ib_extra = IndexBuffer::MakeIndexBuffer(indices_extra, sizeof(indices_extra)/sizeof(unsigned int));

	initialized_GPU = true;
	return true;
}

bool WindLevelEffect::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;
	delete va_extra;
	delete vb_extra;
	delete ib_extra;

	initialized_GPU = false;
	return true;
}

WindLevelEffect::WindLevelEffect(bool transitionWind) {
	tickCount = 0;
	tickCycle = 200;
	currentState = 0;
	maxState = 4;
	if (transitionWind) {
		//stateMultiplier = new double[maxState]{1, .5, 1, .5};
		stateMultiplier = new double[maxState]{.75, .25, .75, .25};
		//TODO: does this look right? (basing this off tricky_maneuvering)
	} else {
		stateMultiplier = new double[maxState]{1, 0, 1, 0};
	}

	initializeGPU();
}

WindLevelEffect::WindLevelEffect() : WindLevelEffect(false) {}

WindLevelEffect::~WindLevelEffect() {
	delete[] stateMultiplier;
}

LevelEffect* WindLevelEffect::factory(GenericFactoryConstructionData& args) {
	//TODO: modify stateMultiplier
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 1) {
			bool* arr = (bool*)(args.getDataPortion(0));
			bool t = arr[0];
			return new WindLevelEffect(t);
		}
	}
	return new WindLevelEffect();
}
