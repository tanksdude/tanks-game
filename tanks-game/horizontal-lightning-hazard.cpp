#include "horizontal-lightning-hazard.h"
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

VertexArray* HorizontalLightningHazard::background_va;
VertexBuffer* HorizontalLightningHazard::background_vb;
IndexBuffer* HorizontalLightningHazard::background_ib;
bool HorizontalLightningHazard::initialized_GPU = false;

std::unordered_map<std::string, float> HorizontalLightningHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

HorizontalLightningHazard::HorizontalLightningHazard(double xpos, double ypos, double width, double height) : RectangularLightningHazard(xpos,ypos,width,height,true) {
	//flexible = false;

	maxBolts = 2;
	//lengthOfBolt = 4;
	bolts.reserve(maxBolts);
	pushDefaultBolt(maxBolts, true);

	//canAcceptPowers = false;

	//modifiesTankCollision = true;
	//hasSpecialEffectTankCollision = true;
	//modifiesBulletCollision = true;
	//hasSpecialEffectBulletCollision = true;

	local_initializeGPU();
	initializeGPU();
}

inline Circle* HorizontalLightningHazard::getLeftPoint() const {
	return new Point(x, y + h/2);
}

inline Circle* HorizontalLightningHazard::getRightPoint() const {
	return new Point(x + w, y + h/2);
}

HorizontalLightningHazard::~HorizontalLightningHazard() {
	//calls ~RectangularLightningHazard(), so this doesn't need to do anything extra
	//clearBolts();

	local_uninitializeGPU(); //I don't know if this is okay, but there isn't an error...
	//uninitializeGPU();
}

bool HorizontalLightningHazard::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	float positions[] = {
		0, 0,   0.5f, 0.5f, 0.5f, 1.0f,
		1, 0,   0.5f, 0.5f, 0.5f, 1.0f,
		1, 1,   0.5f, 0.5f, 0.5f, 1.0f,
		0, 1,   0.5f, 0.5f, 0.5f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	background_vb = VertexBuffer::MakeVertexBuffer(positions, sizeof(positions), RenderingHints::dynamic_draw);
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	background_vb->SetLayout(layout);

	background_ib = IndexBuffer::MakeIndexBuffer(indices, 6);

	background_va = VertexArray::MakeVertexArray();
	background_va->AddVertexBuffer(background_vb);
	background_va->SetIndexBuffer(background_ib);

	initialized_GPU = true;
	return true;
}

//requires a bolt to initialize:
void HorizontalLightningHazard::local_initializeGPU() {
	float* positions = new float[bolts[0]->length*2];
	for (int i = 0; i < bolts[0]->length; i++) {
		positions[i*2]   = bolts[0]->positions[i*2];
		positions[i*2+1] = bolts[0]->positions[i*2+1];
	}
	bolt_vb_length = bolts[0]->length;

	bolt_vb = VertexBuffer::MakeVertexBuffer(positions, bolts[0]->length*2 * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		//{ ShaderDataType::Float4, "a_Color" }
	};
	bolt_vb->SetLayout(layout);

	bolt_va = VertexArray::MakeVertexArray();
	bolt_va->AddVertexBuffer(bolt_vb);

	delete[] positions;
}

void HorizontalLightningHazard::local_reinitializeGPU(int length) { //does not seed the VertexBuffer with values
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

bool HorizontalLightningHazard::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;

	initialized_GPU = false;
	return true;
}

void HorizontalLightningHazard::local_uninitializeGPU() {
	delete bolt_va;
	delete bolt_vb;
}

void HorizontalLightningHazard::streamBoltVertices(const LightningBolt* l) const {
	bolt_vb->modifyData(l->positions.data(), l->length*2 * sizeof(float));
}

RectHazard* HorizontalLightningHazard::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 4) {
			double* arr = (double*)(args.getDataPortion(0));
			double x = arr[0];
			double y = arr[1];
			double w = arr[2];
			double h = arr[3];
			return new HorizontalLightningHazard(x, y, w, h);
		}
	}
	return new HorizontalLightningHazard(0, 0, 0, 0);
}

void HorizontalLightningHazard::specialEffectCircleCollision(Circle* c) {
	//TODO: confirm everything is good
	Circle* leftPoint = getLeftPoint();
	Circle* rightPoint = getRightPoint();
	Circle* circleCenter = new Point(c->x, c->y);
	double intersectionXL, intersectionXR, intersectionYL, intersectionYR;
	int boltPointsL = -1, boltPointsR = -1;

	double xpos, ypos; //circle adjusted x and y

	if (CollisionHandler::fullyCollided(circleCenter, this)) {
		//circle center inside rectangle area
		xpos = c->x;
		ypos = c->y;
	} else {
		//circle center outside rectangle area
		if (c->x < x) {
			xpos = x; //circle too far to the left
		} else if (c->x > x+w) {
			xpos = x+w; //circle too far to the right
		} else {
			xpos = c->x; //circle's x-position is fine
		}
		if (c->y < y) {
			ypos = y; //circle too low
		} else if (c->y > y+h) {
			ypos = y+h; //circle too high
		} else {
			ypos = c->y; //circle's y-position is fine
		}
		//std::cout << "xpos: " << (xpos-c->x) << ", ypos: " << (ypos-c->y) << std::endl;
	}
	if (CollisionHandler::fullyCollided(leftPoint, c)) {
		intersectionXL = c->x;
		intersectionYL = c->y;
		boltPointsL = 2;
	} else {
		std::pair<PositionHolder, PositionHolder> intersections = CollisionHandler::circleLineIntersection(c, xpos, ypos, leftPoint->x, leftPoint->y);
		intersectionXL = std::min(intersections.first.x, intersections.second.x);
		if (c->y < y + h/2) {
			intersectionYL = std::max(intersections.first.y, intersections.second.y);
		} else {
			intersectionYL = std::min(intersections.first.y, intersections.second.y);
		}

		if (intersectionXL < x || intersectionXL > x+w) {
			std::cerr << "WARNING: horizontal lightning endpoint X (left half) out of range!" << std::endl;
			intersectionXL = constrain<double>(intersectionXL, x, x+w);
		}
		if (intersectionYL < y || intersectionYL > y+h) {
			std::cerr << "WARNING: horizontal lightning endpoint Y (left half) out of range!" << std::endl;
			intersectionYL = constrain<double>(intersectionYL, y, y+h);
		}
		boltPointsL = getDefaultNumBoltPoints(sqrt(pow(intersectionXL - leftPoint->x, 2) + pow(intersectionYL - leftPoint->y, 2)));

		//pushBolt(new LightningBolt(0, h/2, intersections.x1 - x, intersections.y1 - y, 2)); //debugging
		//pushBolt(new LightningBolt(0, h/2, intersections.x1 - x, intersections.y2 - y, 2));
		//pushBolt(new LightningBolt(0, h/2, intersections.x2 - x, intersections.y1 - y, 2));
		//pushBolt(new LightningBolt(0, h/2, intersections.x2 - x, intersections.y2 - y, 2));
	}

	if (CollisionHandler::fullyCollided(rightPoint, c)) {
		intersectionXR = c->x;
		intersectionYR = c->y;
		boltPointsR = 2;
	} else {
		std::pair<PositionHolder, PositionHolder> intersections = CollisionHandler::circleLineIntersection(c, xpos, ypos, rightPoint->x, rightPoint->y);
		intersectionXR = std::max(intersections.first.x, intersections.second.x);
		if (c->y < y + h/2) {
			intersectionYR = std::max(intersections.first.y, intersections.second.y);
		} else {
			intersectionYR = std::min(intersections.first.y, intersections.second.y);
		}

		if (intersectionXR < x || intersectionXR > x+w) {
			std::cerr << "WARNING: horizontal lightning endpoint X (right half) out of range!" << std::endl;
			intersectionXR = constrain<double>(intersectionXR, x, x+w);
		}
		if (intersectionYR < y || intersectionYR > y+h) {
			std::cerr << "WARNING: horizontal lightning endpoint Y (right half) out of range!" << std::endl;
			intersectionYR = constrain<double>(intersectionYR, y, y+h);
		}
		boltPointsR = getDefaultNumBoltPoints(sqrt(pow(intersectionXR - rightPoint->x, 2) + pow(intersectionYR - rightPoint->y, 2)));

		//pushBolt(new LightningBolt(intersections.x1 - x, intersections.y1 - y, w, h/2, 2)); //debugging
		//pushBolt(new LightningBolt(intersections.x1 - x, intersections.y2 - y, w, h/2, 2));
		//pushBolt(new LightningBolt(intersections.x2 - x, intersections.y1 - y, w, h/2, 2));
		//pushBolt(new LightningBolt(intersections.x2 - x, intersections.y2 - y, w, h/2, 2));
	}

	//double distL = sqrt(pow(intersectionXL - x, 2) + pow(intersectionYL - (y + h/2), 2));
	//double distR = sqrt(pow((x + w) - intersectionXR, 2) + pow((y + h/2) - intersectionYR, 2));
	//boltPointsL = (boltPointsL < 2 ? getDefaultNumBoltPoints(distL) : boltPointsL);
	//boltPointsR = (boltPointsR < 2 ? getDefaultNumBoltPoints(distR) : boltPointsR);
	pushBolt(new LightningBolt(0, h/2, intersectionXL-x, intersectionYL-y, boltPointsL), false);
	pushBolt(new LightningBolt(intersectionXL-x, intersectionYL-y, intersectionXR-x, intersectionYR-y, 2), false);
	pushBolt(new LightningBolt(intersectionXR-x, intersectionYR-y, w, h/2, boltPointsR), false);
	delete leftPoint; delete rightPoint; delete circleCenter;

	//TODO: refactor? (it's done, right?)
}

void HorizontalLightningHazard::pushBolt(LightningBolt* l, bool simpleRefresh) {
	if (l->length > bolt_vb_length) {
		local_reinitializeGPU(l->length);
	}
	bolts.push_back(l);
	if (simpleRefresh) {
		simpleRefreshBolt(l);
	} else {
		refreshBolt(l);
	}
}

void HorizontalLightningHazard::pushDefaultBolt(int num, bool randomize) {
	for (int i = 0; i < num; i++) {
		LightningBolt* l = new LightningBolt(0, h/2, w, h/2, getDefaultNumBoltPoints(w));
		if (randomize) {
			pushBolt(l, true);
		} else {
			if (l->length > bolt_vb_length) {
				local_reinitializeGPU(l->length);
			}
			bolts.push_back(l);
		}
	}
}

bool HorizontalLightningHazard::validLocation() const {
	bool wallOnLeft = false, wallOnRight = false, wallInMiddle = false;
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		//I'm not sure numerical comparisons are "costly" to the point where short circuiting is needed
		if (!(wallOnLeft && wallOnRight) && (wa->y <= y) && (wa->y + wa->h >= y + h)) {
			if (!wallOnLeft && (x == wa->x + wa->w)) {
				wallOnLeft = true;
			} else if (!wallOnRight && (x + w == wa->x)) {
				wallOnRight = true;
			}
		}
		if (CollisionHandler::partiallyCollidedIgnoreEdge(wa, this)) {
			wallInMiddle = true;
			break;
		}
	}
	if (!wallOnLeft && (x == 0)) {
		wallOnLeft = true;
	}
	if (!wallOnRight && (x + w == GAME_WIDTH)) {
		wallOnRight = true;
	}
	return (wallOnLeft && wallOnRight && !wallInMiddle);
}

bool HorizontalLightningHazard::reasonableLocation() const {
	bool wallOnTop = false, wallOnBottom = false;
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		if ((wa->x <= x) && (wa->x + wa->w >= x + w)) {
			if (!wallOnBottom && (y == wa->y + wa->h)) {
				wallOnBottom = true;
			} else if (!wallOnTop && (y + h == wa->y)) {
				wallOnTop = true;
			}
		}
		if (wallOnTop && wallOnBottom) {
			break;
		}
	}
	if (!wallOnBottom && (y <= 0)) { //should this be ==? //also, should this function return false if the lightning is out of bounds?
		wallOnBottom = true;
	}
	if (!wallOnTop && (y + h >= GAME_HEIGHT)) {
		wallOnTop = true;
	}

	//don't check general lightnings
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		if (CollisionHandler::partiallyCollided(this, HazardManager::getCircleHazard(i))) {
			return false;
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

	return (!(wallOnTop && wallOnBottom) && validLocation());
}

void HorizontalLightningHazard::simpleRefreshBolt(LightningBolt* l) const {
	double maxVariance = h/4;
	/* lightning bolts are allowed to be in an area that looks like this:
	 * 
	 * +   --------------   +        <- h
	 * |  /              \  |
	 * | /                \ |
	 * |<       HERE       >|
	 * | \                / |
	 * |  \              /  |
	 * +   --------------   +        <- 0
	 * 
	 * ^                    ^
	 * 0                    w
	 * 
	 * the region is 1/4 triangle, 1/2 rectangle, then 1/4 triangle
	 */

	//old method:
	/*
	//old method note: x and y coordinates in range [0,1], maxVariance = 1.0/4.0
	for (int j = 1; j < l->length-1; j++) {
		//l->positions[j*2]   = float(j)/(l->length - 1);
		//l->positions[j*2+1] = randFunc();
		double testPoint; //y-position of the new point
		if (j < l->length / 4) { //first quarter
			do {
				testPoint = l->positions[j*2 - 1] + (randFunc()*2-1) * maxVariance;
			} while(testPoint <= -2 * (double(j) / l->length) + .5 || //"below" the triangle (just in slope-intercept form, nothing special)
			        testPoint >=  2 * (double(j) / l->length) + .5);  //"above" the triangle
		} else if (j < l->length * 3.0/4.0) { //middle half
			do {
				testPoint = l->positions[j*2 - 1] + (randFunc()*2-1) * maxVariance;
			} while(testPoint >= 1 || testPoint <= 0);
		} else { //last quarter
			do {
				testPoint = l->positions[j*2 - 1] + (randFunc()*2-1) * maxVariance;
			} while(testPoint <=  2 * (double(j) / l->length - 3.0/4.0) + 0 ||
			        testPoint >= -2 * (double(j) / l->length - 3.0/4.0) + 1);
		}
		l->positions[j*2+1] = testPoint;
	}
	*/

	double deltaX = (l->positions[l->length*2-2] - l->positions[0]) / (l->length - 1);
	for (int j = 1; j < l->length-1; j++) {
		double yRangeLower = l->positions[j*2 - 1] - maxVariance;
		double yRangeUpper = l->positions[j*2 - 1] + maxVariance;
		double yMin, yMax;
		if (j < l->length/4) { //first quarter
			yMin = -2*h/w * (deltaX * j) + h/2;
			yMax =  2*h/w * (deltaX * j) + h/2;
		} else if (j < l->length * 3.0/4.0) { //middle half
			yMin = 0;
			yMax = h;
		} else { //last quarter
			yMin =  2*h/w * (deltaX * (j - l->length*3.0/4.0)) + 0;
			yMax = -2*h/w * (deltaX * (j - l->length*3.0/4.0)) + h;
		}
		yRangeLower = (yRangeLower < yMin ? yMin : yRangeLower);
		yRangeUpper = (yRangeUpper > yMax ? yMax : yRangeUpper);
		l->positions[j*2+1] = yRangeLower + (yRangeUpper - yRangeLower) * RNG::randFunc();
	}
}

void HorizontalLightningHazard::refreshBolt(LightningBolt* l) const {
	RectangularLightningHazard::refreshBolt(l, this->h, this->w);
}

void HorizontalLightningHazard::draw() const {
	drawBackground(false);
	drawBolts();
}

void HorizontalLightningHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(false);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for HorizontalLightningHazard::draw!" << std::endl;
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

void HorizontalLightningHazard::poseDraw() const {
	drawBackground(true);
	drawBolts_Pose();
}

void HorizontalLightningHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(true);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for HorizontalLightningHazard::poseDraw!" << std::endl;
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

void HorizontalLightningHazard::ghostDraw(float alpha) const {
	drawBackground(true, alpha);
	drawBolts_Pose(alpha);
}

void HorizontalLightningHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(true, alpha);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for HorizontalLightningHazard::ghostDraw!" << std::endl;
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

inline void HorizontalLightningHazard::drawBackground(bool pose, float alpha) const {
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
	//scale = scale * scale;

	//main background:
	/*
	//ColorValueHolder color = (pose ? getBackgroundColor_Pose() : getBackgroundColor());
	ColorValueHolder color = getBackgroundColor_Pose();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(w*scale, h, 0, x + (w*(1-scale))/2, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*background_va, *background_ib, *shader);
	*/
	//ColorValueHolder color = (pose ? getBackgroundColor_Pose() : getBackgroundColor());
	ColorValueHolder color = getBackgroundColor_Pose();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

	float coordsAndColor[] = {
		x+(w/2) - (w/2)*scale, y,     color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		x+(w/2) + (w/2)*scale, y,     color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		x+(w/2) + (w/2)*scale, y+h,   color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		x+(w/2) - (w/2)*scale, y+h,   color.getRf(), color.getGf(), color.getBf(), color.getAf()
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	Renderer::SubmitBatchedDraw(coordsAndColor, 4 * (2+4), indices, 2 * 3);

	//outline:
	/*
	glLineWidth(1.0f);

	color = ColorValueHolder(0, 0, 0);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(w, h, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*background_va, *shader, GL_LINES, 0, 4);
	*/

	//ColorValueHolder color_outline = ColorValueHolder(0.0f, 0.0f, 0.0f); //black is a bit too strong for a lightning's outline
	ColorValueHolder color_outline = ColorValueHolder(0.5f, 0.5f, 0.5f);
	color_outline = ColorMixer::mix(BackgroundRect::getBackColor(), color_outline, alpha);
	const float lineWidth = 0.5f;
	//using the same color for the background works well, though it's not used because the outline was added to make the lightning's boundary obvious

	float coordsAndColor_outline[] = {
		//outer
		(x)   - lineWidth, (y)   - lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //0
		(x+w) + lineWidth, (y)   - lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //1
		(x+w) + lineWidth, (y+h) + lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //2
		(x)   - lineWidth, (y+h) + lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //3

		//inner
		(x)   - lineWidth, (y)   + lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //4
		(x+w) + lineWidth, (y)   + lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //5
		(x+w) + lineWidth, (y+h) - lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //6
		(x)   - lineWidth, (y+h) - lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf()  //7
	};
	unsigned int indices_outline[] = { //rects
		//bottom
		0, 1, 5,
		5, 4, 0,

		//top
		2, 3, 7,
		7, 6, 2
	};

	Renderer::SubmitBatchedDraw(coordsAndColor_outline, (4+4) * (2+4), indices_outline, (2*2) * 3);
}

inline void HorizontalLightningHazard::drawBolts(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	if (!currentlyActive) {
		return;
	}

	//glLineWidth(2.0f);

	/*
	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(1, 1, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	for (int i = 0; i < bolts.size(); i++) {
		//I think the VertexBuffer resizing should happen here, but there would probably be less strain if it happens only when a bullet/tank collides
		//TODO: that ^ should be the preferred way, since only draw() (and initializeGPU()) should do GPU stuff
		//if (bolts[i]->length > bolt_vb_length) {
		//	local_reinitializeGPU(bolts[i]->length);
		//}
		streamBoltVertices(bolts[i]); //TODO: fix
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, bolts[i]->length);
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	*/

	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 0.75f;

	for (int i = 0; i < bolts.size(); i++) {
		float* coordsAndColor = new float[(bolts[i]->length-1)*4*(2+4)];
		unsigned int* indices = new unsigned int[(bolts[i]->length-1)*6];

		for (int j = 0; j < bolts[i]->length-1; j++) {
			const int startVertex = (j*4) * 6;
			const int startIndex = j*6;

			SimpleVector2D dist = SimpleVector2D(bolts[i]->positions[(j+1)*2] - bolts[i]->positions[j*2], bolts[i]->positions[(j+1)*2+1] - bolts[i]->positions[j*2+1]);
			SimpleVector2D distCW = SimpleVector2D(dist.getAngle() - PI/2, lineWidth, true);

			coordsAndColor[startVertex + 0*6]   = x + bolts[i]->positions[j*2]                     + distCW.getXComp();
			coordsAndColor[startVertex + 0*6+1] = y + bolts[i]->positions[j*2+1]                   + distCW.getYComp();
			coordsAndColor[startVertex + 1*6]   = x + bolts[i]->positions[j*2]   + dist.getXComp() + distCW.getXComp();
			coordsAndColor[startVertex + 1*6+1] = y + bolts[i]->positions[j*2+1] + dist.getYComp() + distCW.getYComp();
			coordsAndColor[startVertex + 2*6]   = x + bolts[i]->positions[j*2]   + dist.getXComp() - distCW.getXComp();
			coordsAndColor[startVertex + 2*6+1] = y + bolts[i]->positions[j*2+1] + dist.getYComp() - distCW.getYComp();
			coordsAndColor[startVertex + 3*6]   = x + bolts[i]->positions[j*2]                     - distCW.getXComp();
			coordsAndColor[startVertex + 3*6+1] = y + bolts[i]->positions[j*2+1]                   - distCW.getYComp();

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

		Renderer::SubmitBatchedDraw(coordsAndColor, (bolts[i]->length-1)*4*(2+4), indices, (bolts[i]->length-1)*6);
		delete[] coordsAndColor; delete[] indices;
	}
}

inline void HorizontalLightningHazard::drawBolts_Pose(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	//glLineWidth(2.0f);

	/*
	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(1, 1, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);
	*/

	//generate bolts
	std::vector<LightningBolt*> poseBolts;
	//from pushDefaultBolt(), mostly
	LightningBolt* l = new LightningBolt(0, h/2, w, h/2, getDefaultNumBoltPoints(w));

	if (l->length > bolt_vb_length) {
		//cut off the parts that won't fit; shouldn't happen though
		l->length = bolt_vb_length;
	}
	refreshBolt(l);

	//draw
	/*
	for (int i = 0; i < poseBolts.size(); i++) {
		//match with drawBolts()
		streamBoltVertices(poseBolts[i]);
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, poseBolts[i]->length);
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	*/
	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 0.75f; //TODO: too thick?

	for (int i = 0; i < poseBolts.size(); i++) {
		//match with drawBolts()
		float* coordsAndColor = new float[(poseBolts[i]->length-1)*4*(2+4)];
		unsigned int* indices = new unsigned int[(poseBolts[i]->length-1)*6];

		for (int j = 0; j < poseBolts[i]->length-1; j++) {
			const int startVertex = (j*4) * 6;
			const int startIndex = j*6;

			SimpleVector2D dist = SimpleVector2D(poseBolts[i]->positions[(j+1)*2] - poseBolts[i]->positions[j*2], poseBolts[i]->positions[(j+1)*2+1] - poseBolts[i]->positions[j*2+1]);
			SimpleVector2D distCW = SimpleVector2D(dist.getAngle() - PI/2, lineWidth, true);

			coordsAndColor[startVertex + 0*6]   = x + poseBolts[i]->positions[j*2]                     + distCW.getXComp();
			coordsAndColor[startVertex + 0*6+1] = y + poseBolts[i]->positions[j*2+1]                   + distCW.getYComp();
			coordsAndColor[startVertex + 1*6]   = x + poseBolts[i]->positions[j*2]   + dist.getXComp() + distCW.getXComp();
			coordsAndColor[startVertex + 1*6+1] = y + poseBolts[i]->positions[j*2+1] + dist.getYComp() + distCW.getYComp();
			coordsAndColor[startVertex + 2*6]   = x + poseBolts[i]->positions[j*2]   + dist.getXComp() - distCW.getXComp();
			coordsAndColor[startVertex + 2*6+1] = y + poseBolts[i]->positions[j*2+1] + dist.getYComp() - distCW.getYComp();
			coordsAndColor[startVertex + 3*6]   = x + poseBolts[i]->positions[j*2]                     - distCW.getXComp();
			coordsAndColor[startVertex + 3*6+1] = y + poseBolts[i]->positions[j*2+1]                   - distCW.getYComp();

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

		Renderer::SubmitBatchedDraw(coordsAndColor, (poseBolts[i]->length-1)*4*(2+4), indices, (poseBolts[i]->length-1)*6);
		delete[] coordsAndColor; delete[] indices;
	}

	for (int i = 0; i < poseBolts.size(); i++) {
		delete poseBolts[i];
	}
}

RectHazard* HorizontalLightningHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData& args) {
	//minimum/maximum width and height not in argv
	if (WallManager::getNumWalls() == 0) {
		return nullptr; //don't bother trying to see if a horizontal lightning could go from edge to edge
	}

	int attempts = 0;
	RectHazard* randomized = nullptr;
	double xpos, ypos, width, height;
	double minWidth = 40, maxWidth = 160;

	do {
		height = RNG::randFunc() * (24 - 12) + 12;
		for (int i = 0; i < WallManager::getNumWalls(); i++) {
			Wall* wa = WallManager::getWall(i);
			xpos = wa->x + wa->w;
			ypos = wa->y + RNG::randFunc() * constrain<double>(wa->h - height, 0, wa->h);
			int j, wallAttempts = 0;
			do {
				j = RNG::randFunc() * WallManager::getNumWalls();
				wallAttempts++;
			} while ((wallAttempts < 8) && (j == i));
			if (j != i) {
				Wall* otherWall = WallManager::getWall(j);
				width = otherWall->x - xpos;
			} else {
				width = RNG::randFunc() * (maxWidth - minWidth) + minWidth;
			}
		}
		if ((xpos >= x_start) && (xpos + width <= x_start + area_width) && (ypos >= y_start) && (ypos + height <= y_start + area_height) && (width <= maxWidth) && (width >= minWidth)) {
			RectHazard* testHorizontalLightning = new HorizontalLightningHazard(xpos, ypos, width, height);
			if (testHorizontalLightning->reasonableLocation()) {
				randomized = testHorizontalLightning;
				break;
			} else {
				delete testHorizontalLightning;
			}
		}
		attempts++;
	} while (attempts < 32);

	return randomized;
}
