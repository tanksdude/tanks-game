#include "circular-lightning-hazard.h"
#include "renderer.h"
#include "background-rect.h"
#include "color-mixer.h"
#include "mylib.h"
#include "constants.h"
#include <cmath>
#include <stdexcept>
#include <algorithm> //std::copy
#include "point.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "collision-handler.h"
#include "rng.h"
#include <iostream>

VertexArray* CircularLightningHazard::background_va;
VertexBuffer* CircularLightningHazard::background_vb;
IndexBuffer* CircularLightningHazard::background_ib;
bool CircularLightningHazard::initialized_GPU = false;

std::unordered_map<std::string, float> CircularLightningHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

CircularLightningHazard::CircularLightningHazard(double xpos, double ypos, double radius) : CircleHazard(HAZARD_TEAM) {
	x = xpos;
	y = ypos;
	r = radius;

	tickCount = 0;
	tickCycle = 100; //100 is JS default (because of power speed)
	double temp[2] = { 2, 2 };
	std::copy(temp, temp+2, stateMultiplier);
	currentlyActive = false;
	//flexible = false;

	maxBolts = 1;
	lengthOfBolt = 4;
	bolts.reserve(maxBolts);
	pushDefaultBolt(maxBolts, true); //there isn't really a default bolt...
	boltTick = 0;
	boltCycle = 4;
	boltsNeeded = false;

	canAcceptPowers = false;

	modifiesTankCollision = true;
	hasSpecialEffectTankCollision = true;
	modifiesBulletCollision = true;
	hasSpecialEffectBulletCollision = true;

	local_initializeGPU();
	initializeGPU();
}

inline Circle* CircularLightningHazard::getCenterPoint() const {
	return new Point(x, y);
}

CircularLightningHazard::~CircularLightningHazard() {
	//clearBolts();

	local_uninitializeGPU();
	//uninitializeGPU();
}

bool CircularLightningHazard::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	float positions[(Circle::numOfSides+1)*(2+4)];
	positions[0] = 0;
	positions[1] = 0;
	positions[2] = 0.5f;
	positions[3] = 0.5f;
	positions[4] = 0.5f;
	positions[5] = 1.0f;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		positions[i*6]   = cos((i-1) * 2*PI / Circle::numOfSides);
		positions[i*6+1] = sin((i-1) * 2*PI / Circle::numOfSides);
		positions[i*6+2] = 0.5f;
		positions[i*6+3] = 0.5f;
		positions[i*6+4] = 0.5f;
		positions[i*6+5] = 1.0f;
	}

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = 0;
		indices[i*3+1] = i+1;
		indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	background_vb = VertexBuffer::MakeVertexBuffer(positions, (Circle::numOfSides+1)*(2+4) * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	background_vb->SetLayout(layout);

	background_ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	background_va = VertexArray::MakeVertexArray();
	background_va->AddVertexBuffer(background_vb);
	background_va->SetIndexBuffer(background_ib);

	initialized_GPU = true;
	return true;
}

//requires a bolt to initialize:
void CircularLightningHazard::local_initializeGPU() {
	float* positions = new float[bolts[0]->length*2];
	for (int i = 0; i < bolts[0]->length; i++) {
		positions[i*2]   = bolts[0]->positions[i*2];
		positions[i*2+1] = bolts[0]->positions[i*2+1];
	}
	bolt_vb_length = bolts[0]->length;

	bolt_vb = VertexBuffer::MakeVertexBuffer(positions, bolts[0]->length*2 * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		//{ ShaderDataType::Float4, "a_Color" } //TODO
	};
	bolt_vb->SetLayout(layout);

	bolt_va = VertexArray::MakeVertexArray();
	bolt_va->AddVertexBuffer(bolt_vb);

	delete[] positions;
}

void CircularLightningHazard::local_reinitializeGPU(int length) { //does not seed the VertexBuffer with values
	delete bolt_va;
	delete bolt_vb;

	float* positions = new float[length*2];
	bolt_vb_length = length;

	bolt_vb = VertexBuffer::MakeVertexBuffer(positions, length*2 * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		//{ ShaderDataType::Float4, "a_Color" }
	};
	bolt_vb->SetLayout(layout);

	bolt_va = VertexArray::MakeVertexArray();
	bolt_va->AddVertexBuffer(bolt_vb);

	delete[] positions;
}

bool CircularLightningHazard::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;

	initialized_GPU = false;
	return true;
}

void CircularLightningHazard::local_uninitializeGPU() {
	delete bolt_va;
	delete bolt_vb;
}

void CircularLightningHazard::streamBoltVertices(const LightningBolt* l) const {
	bolt_vb->modifyData(l->positions.data(), l->length*2 * sizeof(float));
}

CircleHazard* CircularLightningHazard::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 3) {
			double* arr = (double*)(args.getDataPortion(0));
			double x = arr[0];
			double y = arr[1];
			double r = arr[2];
			return new CircularLightningHazard(x, y, r);
		}
	}
	return new CircularLightningHazard(0, 0, 0);
}

void CircularLightningHazard::specialEffectCircleCollision(Circle* c) {
	//it's so nice how simple this is
	Circle* centerPoint = getCenterPoint();
	double intersectionX, intersectionY;
	int boltPoints = -1;
	if (CollisionHandler::fullyCollided(centerPoint, c)) {
		intersectionX = c->x;
		intersectionY = c->y;
		boltPoints = 2;
	} else {
		//find angle from center to circle's center
		double angle = atan2(c->y - centerPoint->y, c->x - centerPoint->x);
		//find distance from center to center, minus circle's radius
		double d = sqrt(pow(c->x - centerPoint->x, 2) + pow(c->y - centerPoint->y, 2)) - c->r;
		if (d < 0) {
			throw std::domain_error("ERROR: distance on circular lightning is wrong!");
			//is is really a "domain error?"
		}
		intersectionX = centerPoint->x + cos(angle) * d;
		intersectionY = centerPoint->y + sin(angle) * d;
		boltPoints = getDefaultNumBoltPoints(d);
	}
	//double dist = sqrt(pow(intersectionX - centerPoint->x, 2) + pow(intersectionY - centerPoint->y, 2));
	//std::cout << dist << std::endl;
	//boltPoints = (boltPoints < 2 ? getDefaultNumBoltPoints(dist) : boltPoints); //no need
	pushBolt(new LightningBolt(0, 0, intersectionX - x, intersectionY - y, boltPoints)); //TODO: is this right? (probably)
	delete centerPoint;
}

void CircularLightningHazard::specialEffectTankCollision(Tank* t) {
	//if bolts are being randomized, clear them, and mark that they've been cleared
	if (!boltsNeeded) {
		clearBolts();
	}
	boltsNeeded = true;

	if (std::find(targetedObjects.begin(), targetedObjects.end(), t->getGameID()) == targetedObjects.end()) {
		targetedObjects.push_back(t->getGameID());
	} else {
		return;
	}

	specialEffectCircleCollision(t);
}

void CircularLightningHazard::specialEffectBulletCollision(Bullet* b) {
	if (!boltsNeeded) {
		clearBolts();
	}
	boltsNeeded = true;

	if (std::find(targetedObjects.begin(), targetedObjects.end(), b->getGameID()) == targetedObjects.end()) {
		targetedObjects.push_back(b->getGameID());
	} else {
		return;
	}

	specialEffectCircleCollision(b);
}

void CircularLightningHazard::pushBolt(LightningBolt* l) {
	if (l->length > bolt_vb_length) {
		local_reinitializeGPU(l->length);
	}
	bolts.push_back(l);
	refreshBolt(l);
}

void CircularLightningHazard::pushDefaultBolt(int num, bool randomize) {
	//the default bolt is from center to a random point
	double randR = r*RNG::randFunc(), randAngle = 2*PI*RNG::randFunc();
	double xEnd = randR*cos(randAngle), yEnd = randR*sin(randAngle);
	for (int i = 0; i < num; i++) {
		LightningBolt* l = new LightningBolt(0, 0, xEnd, yEnd, getDefaultNumBoltPoints(sqrt(pow(xEnd - 0, 2) + pow(yEnd - 0, 2))));
		if (randomize) {
			pushBolt(l);
		} else {
			if (l->length > bolt_vb_length) {
				local_reinitializeGPU(l->length);
			}
			bolts.push_back(l);
		}
	}
}

bool CircularLightningHazard::validLocation() const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		if (CollisionHandler::partiallyCollidedIgnoreEdge(wa, this)) {
			return false;
		}
	}
	return true;
}

bool CircularLightningHazard::reasonableLocation() const {
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		CircleHazard* ch = HazardManager::getCircleHazard(i);
		if (ch->getName() != this->getName()) {
			if (CollisionHandler::partiallyCollided(this, ch)) {
				return false;
			}
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		RectHazard* rh = HazardManager::getRectHazard(i);
		if (rh->getName() != this->getName()) {
			if (CollisionHandler::partiallyCollided(this, rh)) {
				return false;
			}
		}
	}

	return validLocation();
}

void CircularLightningHazard::refreshBolt(LightningBolt* l) const {
	//TODO: more testing
	//see RectangularLightningHazard
	if (l->length <= 2) {
		return;
	}

	float deltaX = l->positions[l->length*2-2] - l->positions[0];
	float deltaY = l->positions[l->length*2-1] - l->positions[1];
	double dist = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	double rotationAngle = atan2(deltaY, deltaX);
	double angleSin = sin(rotationAngle);
	double angleCos = cos(rotationAngle);
	double newH = dist * 1; //maybe *.5, I dunno
	double maxVariance = 1.0/4.0 * dist * 1; //(same here)

	float polygonX[6] = {
		l->positions[0],
		l->positions[0] + deltaX * 1.0/4.0 - angleSin * newH * .5,
		l->positions[0] + deltaX * 3.0/4.0 - angleSin * newH * .5,
		l->positions[0] + deltaX,
		l->positions[0] + deltaX * 3.0/4.0 + angleSin * newH * .5,
		l->positions[0] + deltaX * 1.0/4.0 + angleSin * newH * .5
	};
	float polygonY[6] = {
		l->positions[1],
		l->positions[1] + deltaY * 1.0/4.0 + angleCos * newH * .5,
		l->positions[1] + deltaY * 3.0/4.0 + angleCos * newH * .5,
		l->positions[1] + deltaY,
		l->positions[1] + deltaY * 3.0/4.0 - angleCos * newH * .5,
		l->positions[1] + deltaY * 1.0/4.0 - angleCos * newH * .5
	};

	//std::cout << "deltaX: " << deltaX << std::endl;
	//std::cout << "deltaY: " << deltaY << std::endl;
	//std::cout << "deltaY adj: " << (deltaY * 1/1) << std::endl;
	//std::cout << "dist: " << dist << std::endl;
	//std::cout << "angle: " << (rotationAngle * 180/3.1415926535897) << std::endl;
	//std::cout << "cos(angle): " << angleCos << std::endl;
	//std::cout << "sin(angle): " << angleSin << std::endl;
	//for (int i = 0; i < 6; i++) {
	//	std::cout << i << ": " << polygonX[i] << " " << polygonY[i] << std::endl;
	//}

	for (int j = 1; j < l->length-1; j++) {
		double randTemp;
		float testY, testX;
		do {
			randTemp = (RNG::randFunc()*2-1)*maxVariance;
			testY = l->positions[j*2 - 1] + (deltaY/(l->length-1)) + randTemp * angleCos;
			testX = l->positions[j*2 - 2] + (deltaX/(l->length-1)) - randTemp * angleSin;
			//std::cout << testX << " " << testY << std::endl;
		} while (sqrt(pow(testY,2) + pow(testX,2)) > r || !pointInPolygon(6, polygonX, polygonY, testX, testY));
		//the first case is rare, but I'm fairly certain it's a useless check if pointInPolygon is checked first
		l->positions[j*2]   = testX;
		l->positions[j*2+1] = testY;
	}
}

void CircularLightningHazard::draw() const {
	drawBackground(false);
	drawBolts();
}

void CircularLightningHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(false);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularLightningHazard::draw!" << std::endl;
		case DrawingLayers::normal:
			drawBolts();
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

void CircularLightningHazard::poseDraw() const {
	drawBackground(true);
	drawBolts_Pose();
}

void CircularLightningHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(true);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularLightningHazard::poseDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBolts_Pose();
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

void CircularLightningHazard::ghostDraw(float alpha) const {
	//not too sure on this
	drawBackground(true, alpha);
	drawBolts_Pose(alpha);
}

void CircularLightningHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(true, alpha);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularLightningHazard::ghostDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBolts_Pose(alpha);
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

inline void CircularLightningHazard::drawBackground(bool pose, float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	double scale;
	if (pose || currentlyActive) {
		scale = 1.0;
	} else {
		scale = tickCount / (tickCycle * stateMultiplier[currentlyActive]);
	}
	scale = scale * scale;

	//main background:
	/*
	//ColorValueHolder color = (pose ? getBackgroundColor_Pose() : getBackgroundColor());
	ColorValueHolder color = getBackgroundColor_Pose();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r*scale, r*scale, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*background_va, *background_ib, *shader);
	*/

	//ColorValueHolder color = (pose ? getBackgroundColor_Pose() : getBackgroundColor());
	ColorValueHolder color = getBackgroundColor_Pose();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

	float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
	coordsAndColor[0] = x;
	coordsAndColor[1] = y;
	coordsAndColor[2] = color.getRf();
	coordsAndColor[3] = color.getGf();
	coordsAndColor[4] = color.getBf();
	coordsAndColor[5] = color.getAf();
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		coordsAndColor[i*6]   = x + (r*scale) * cos((i-1) * 2*PI / Circle::numOfSides);
		coordsAndColor[i*6+1] = y + (r*scale) * sin((i-1) * 2*PI / Circle::numOfSides);
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

	//outline:
	/*
	glLineWidth(1.0f);

	color = ColorValueHolder(0, 0, 0);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*background_va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);
	*/

	//ColorValueHolder color_outline = ColorValueHolder(0.0f, 0.0f, 0.0f); //black is a bit too strong for a lightning's outline
	ColorValueHolder color_outline = ColorValueHolder(0.5f, 0.5f, 0.5f);
	color_outline = ColorMixer::mix(BackgroundRect::getBackColor(), color_outline, alpha);
	const float lineWidth = 0.5f;
	//using the same color for the background works well, though it's not used because the outline was added to make the lightning's boundary obvious

	float coordsAndColor_outline[(Circle::numOfSides*2)*(2+4)];
	for (int i = 0; i < Circle::numOfSides; i++) {
		coordsAndColor_outline[(i*2)  *6]   = x + (r-lineWidth) * cos(i * 2*PI / Circle::numOfSides);
		coordsAndColor_outline[(i*2)  *6+1] = y + (r-lineWidth) * sin(i * 2*PI / Circle::numOfSides);
		coordsAndColor_outline[(i*2+1)*6]   = x + (r+lineWidth) * cos(i * 2*PI / Circle::numOfSides);
		coordsAndColor_outline[(i*2+1)*6+1] = y + (r+lineWidth) * sin(i * 2*PI / Circle::numOfSides);

		coordsAndColor_outline[(i*2)  *6+2] = color_outline.getRf();
		coordsAndColor_outline[(i*2)  *6+3] = color_outline.getGf();
		coordsAndColor_outline[(i*2)  *6+4] = color_outline.getBf();
		coordsAndColor_outline[(i*2)  *6+5] = color_outline.getAf();
		coordsAndColor_outline[(i*2+1)*6+2] = color_outline.getRf();
		coordsAndColor_outline[(i*2+1)*6+3] = color_outline.getGf();
		coordsAndColor_outline[(i*2+1)*6+4] = color_outline.getBf();
		coordsAndColor_outline[(i*2+1)*6+5] = color_outline.getAf();
	}

	unsigned int indices_outline[Circle::numOfSides*6];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices_outline[i*6]   =  i*2;
		indices_outline[i*6+1] =  i*2+1;
		indices_outline[i*6+2] = (i*2+3) % (Circle::numOfSides*2);
		indices_outline[i*6+3] = (i*2+3) % (Circle::numOfSides*2);
		indices_outline[i*6+4] = (i*2+2) % (Circle::numOfSides*2);
		indices_outline[i*6+5] =  i*2;
	}

	Renderer::SubmitBatchedDraw(coordsAndColor_outline, (Circle::numOfSides*2)*(2+4), indices_outline, Circle::numOfSides*6);
}

inline void CircularLightningHazard::drawBolts(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	if (!currentlyActive) {
		return;
	}

	glLineWidth(2.0f);

	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(1, 1, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	for (int i = 0; i < bolts.size(); i++) {
		//I think the VertexBuffer resizing should happen here, but there would probably be less strain if it happens only when a bullet/tank collides
		//TODO: that ^ should be the preferred way, since only draw() (and initializeGPU()) should do GPU stuff
		/*
		if (bolts[i]->length > bolt_vb_length) {
			local_reinitializeGPU(bolts[i]->length);
		}
		*/
		streamBoltVertices(bolts[i]); //TODO: fix
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, bolts[i]->length);
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void CircularLightningHazard::drawBolts_Pose(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	glLineWidth(2.0f);

	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(1, 1, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	//generate bolts
	std::vector<LightningBolt*> poseBolts;
	for (int i = 0; i < 4; i++) {
		//from pushDefaultBolt(), mostly
		double dist = r * .75, angle = PI/4 + i*PI/2;
		double xEnd = dist*cos(angle), yEnd = dist*sin(angle);
		LightningBolt* l = new LightningBolt(0, 0, xEnd, yEnd, getDefaultNumBoltPoints(sqrt(pow(xEnd - 0, 2) + pow(yEnd - 0, 2))));

		if (l->length > bolt_vb_length) {
			//cut off the parts that won't fit; shouldn't happen though
			l->length = bolt_vb_length;
		}
		refreshBolt(l);
	}

	//draw
	for (int i = 0; i < poseBolts.size(); i++) {
		//match with drawBolts()
		streamBoltVertices(poseBolts[i]);
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, poseBolts[i]->length);
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

CircleHazard* CircularLightningHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData& args) {
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
			radius = RNG::randFunc() * (60 - 30) + 30; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc() * (area_width - 2*radius) + (x_start + radius);
		ypos = RNG::randFunc() * (area_height - 2*radius) + (y_start + radius);
		CircleHazard* testCircularLightning = new CircularLightningHazard(xpos, ypos, radius);
		if (testCircularLightning->reasonableLocation()) {
			randomized = testCircularLightning;
			break;
		} else {
			delete testCircularLightning;
		}
		attempts++;
	} while (attempts < 32);

	return randomized;
}
