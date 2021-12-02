#include "verticallightning.h"
#include "gamemanager.h"
#include "renderer.h"
#include "backgroundrect.h"
#include "colormixer.h"
#include "mylib.h"
#include "constants.h"
#include <math.h>
#include <stdexcept>
#include <algorithm> //std::copy
#include "point.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "collisionhandler.h"
#include "rng.h"
#include <iostream>

VertexArray* VerticalLightning::background_va;
VertexBuffer* VerticalLightning::background_vb;
IndexBuffer* VerticalLightning::background_ib;
bool VerticalLightning::initialized_GPU = false;

std::unordered_map<std::string, float> VerticalLightning::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "random", .5f });
	return weights;
}

VerticalLightning::VerticalLightning(double xpos, double ypos, double width, double height) : RectangularLightning(xpos,ypos,width,height,true) {
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

Circle* VerticalLightning::getTopPoint() const {
	return new Point(x + w/2, y + h);
}

Circle* VerticalLightning::getBottomPoint() const {
	return new Point(x + w/2, y);
}

VerticalLightning::~VerticalLightning() {
	//calls ~RectangularLightning(), so this doesn't need to do anything extra
	//clearBolts();

	local_uninitializeGPU(); //I don't know if this is okay, but there isn't an error...
	//uninitializeGPU();
}

bool VerticalLightning::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

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

	background_vb = VertexBuffer::MakeVertexBuffer(positions, 4*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout(2);
	background_va = VertexArray::MakeVertexArray(*background_vb, layout);

	background_ib = IndexBuffer::MakeIndexBuffer(indices, 6);

	initialized_GPU = true;
	return true;
}

//requires a bolt to initialize:
void VerticalLightning::local_initializeGPU() {
	float* positions = new float[bolts[0]->length*2];
	for (int i = 0; i < bolts[0]->length; i++) {
		positions[i*2]   = bolts[0]->positions[i*2];
		positions[i*2+1] = bolts[0]->positions[i*2+1];
	}
	bolt_vb_length = bolts[0]->length;

	bolt_vb = VertexBuffer::MakeVertexBuffer(positions, bolts[0]->length*2 * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout(2);
	bolt_va = VertexArray::MakeVertexArray(*bolt_vb, layout);

	delete[] positions;
}

void VerticalLightning::local_reinitializeGPU(int length) { //does not seed the VertexBuffer with values
	delete bolt_va;
	delete bolt_vb;

	float* positions = new float[length*2];
	bolt_vb_length = length;

	bolt_vb = VertexBuffer::MakeVertexBuffer(positions, length*2 * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout(2);
	bolt_va = VertexArray::MakeVertexArray(*bolt_vb, layout);

	delete[] positions;
}

bool VerticalLightning::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;

	initialized_GPU = false;
	return true;
}

void VerticalLightning::local_uninitializeGPU() {
	delete bolt_va;
	delete bolt_vb;
}

void VerticalLightning::streamBoltVertices(unsigned int boltNum) const {
	bolt_vb->modifyData(bolts[boltNum]->positions.data(), bolts[boltNum]->length*2 * sizeof(float));
}

RectHazard* VerticalLightning::factory(int argc, std::string* argv) {
	if (argc >= 4) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double w = std::stod(argv[2]);
		double h = std::stod(argv[3]);
		return new VerticalLightning(x, y, w, h);
	}
	return new VerticalLightning(0, 0, 0, 0);
}

void VerticalLightning::specialEffectCircleCollision(Circle* c) {
	//TODO: confirm everything is good
	Circle* bottomPoint = getBottomPoint();
	Circle* topPoint = getTopPoint();
	Circle* circleCenter = new Point(c->x, c->y);
	double intersectionXD, intersectionXU, intersectionYD, intersectionYU;
	int boltPointsD = -1, boltPointsU = -1;

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
	if (CollisionHandler::fullyCollided(bottomPoint, c)) {
		intersectionXD = c->x;
		intersectionYD = c->y;
		boltPointsD = 2;
	} else {
		std::pair<PositionHolder, PositionHolder> intersections = CollisionHandler::circleLineIntersection(c, xpos, ypos, bottomPoint->x, bottomPoint->y);
		if (c->x < x + w/2) {
			intersectionXD = std::max(intersections.first.x, intersections.second.x);
		} else {
			intersectionXD = std::min(intersections.first.x, intersections.second.x);
		}
		intersectionYD = std::min(intersections.first.y, intersections.second.y);

		if (intersectionXD < x || intersectionXD > x+w) {
			std::cerr << "WARNING: vertical lightning endpoint X (bottom half) out of range!" << std::endl;
			intersectionXD = constrain<double>(intersectionXD, x, x+w);
		}
		if (intersectionYD < y || intersectionYD > y+h) {
			std::cerr << "WARNING: vertical lightning endpoint Y (bottom half) out of range!" << std::endl;
			intersectionYD = constrain<double>(intersectionYD, y, y+h);
		}
		boltPointsD = getDefaultNumBoltPoints(sqrt(pow(intersectionXD - bottomPoint->x, 2) + pow(intersectionYD - bottomPoint->y, 2)));

		//pushBolt(new LightningBolt(w/2, 0, intersections.x1 - x, intersections.y1 - y, 2)); //debugging
		//pushBolt(new LightningBolt(w/2, 0, intersections.x1 - x, intersections.y2 - y, 2));
		//pushBolt(new LightningBolt(w/2, 0, intersections.x2 - x, intersections.y1 - y, 2));
		//pushBolt(new LightningBolt(w/2, 0, intersections.x2 - x, intersections.y2 - y, 2));
	}

	if (CollisionHandler::fullyCollided(topPoint, c)) {
		intersectionXU = c->x;
		intersectionYU = c->y;
		boltPointsU = 2;
	} else {
		std::pair<PositionHolder, PositionHolder> intersections = CollisionHandler::circleLineIntersection(c, xpos, ypos, topPoint->x, topPoint->y);
		if (c->x < x + w/2) {
			intersectionXU = std::max(intersections.first.x, intersections.second.x);
		} else {
			intersectionXU = std::min(intersections.first.x, intersections.second.x);
		}
		intersectionYU = std::max(intersections.first.y, intersections.second.y);

		if (intersectionXU < x || intersectionXU > x+w) {
			std::cerr << "WARNING: vertical lightning endpoint X (top half) out of range!" << std::endl;
			intersectionXU = constrain<double>(intersectionXU, x, x+w);
		}
		if (intersectionYU < y || intersectionYU > y+h) {
			std::cerr << "WARNING: vertical lightning endpoint Y (top half) out of range!" << std::endl;
			intersectionYU = constrain<double>(intersectionYU, y, y+h);
		}
		boltPointsU = getDefaultNumBoltPoints(sqrt(pow(intersectionXU - topPoint->x, 2) + pow(intersectionYU - topPoint->y, 2)));

		//pushBolt(new LightningBolt(intersections.x1 - x, intersections.y1 - y, w/2, h, 2)); //debugging
		//pushBolt(new LightningBolt(intersections.x1 - x, intersections.y2 - y, w/2, h, 2));
		//pushBolt(new LightningBolt(intersections.x2 - x, intersections.y1 - y, w/2, h, 2));
		//pushBolt(new LightningBolt(intersections.x2 - x, intersections.y2 - y, w/2, h, 2));
	}

	//double distL = sqrt(pow(intersectionXD - (x + w/2), 2) + pow(intersectionYD - y, 2));
	//double distR = sqrt(pow((x + w/2) - intersectionXU, 2) + pow((y + h) - intersectionYU, 2));
	//boltPointsD = (boltPointsD < 2 ? getDefaultNumBoltPoints(distL) : boltPointsD);
	//boltPointsU = (boltPointsU < 2 ? getDefaultNumBoltPoints(distR) : boltPointsU);
	pushBolt(new LightningBolt(w/2, 0, intersectionXD-x, intersectionYD-y, boltPointsD), false);
	pushBolt(new LightningBolt(intersectionXD-x, intersectionYD-y, intersectionXU-x, intersectionYU-y, 2), false);
	pushBolt(new LightningBolt(intersectionXU-x, intersectionYU-y, w/2, h, boltPointsU), false);
	delete bottomPoint, topPoint, circleCenter;

	//TODO: refactor? (it's done, right?)
}

void VerticalLightning::pushBolt(LightningBolt* l, bool simpleRefresh) {
	if (l->length > bolt_vb_length) {
		local_reinitializeGPU(l->length);
	}
	bolts.push_back(l);
	if (simpleRefresh) {
		simpleRefreshBolt(bolts.size() - 1);
	} else {
		refreshBolt(bolts.size() - 1);
	}
}

void VerticalLightning::pushDefaultBolt(int num, bool randomize) {
	for (int i = 0; i < num; i++) {
		LightningBolt* l = new LightningBolt(w/2, 0, w/2, h, getDefaultNumBoltPoints(h));
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

bool VerticalLightning::validLocation() const {
	bool wallOnTop = false, wallOnBottom = false, wallInMiddle = false;
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		//I'm not sure numerical comparisons are "costly" to the point where short circuiting is needed
		if (!(wallOnTop && wallOnBottom) && (wa->x <= x) && (wa->x + wa->w >= x + w)) {
			if (!wallOnTop && (y + h == wa->y)) {
				wallOnTop = true;
			} else if (!wallOnBottom && (y == wa->y + wa->h)) {
				wallOnBottom = true;
			}
		}
		if (CollisionHandler::partiallyCollidedIgnoreEdge(wa, this)) {
			wallInMiddle = true;
			break;
		}
	}
	if (!wallOnTop && (y + h == GAME_HEIGHT)) {
		wallOnTop = true;
	}
	if (!wallOnBottom && (y == 0)) {
		wallOnBottom = true;
	}
	return (wallOnTop && wallOnBottom && !wallInMiddle);
}

bool VerticalLightning::reasonableLocation() const {
	bool wallOnLeft = false, wallOnRight = false;
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		if ((wa->y <= y) && (wa->y + wa->y >= y + h)) {
			if (!wallOnLeft && (x == wa->x + wa->w)) {
				wallOnLeft = true;
			} else if (!wallOnRight && (x + w == wa->x)) {
				wallOnRight = true;
			}
		}
		if (wallOnLeft && wallOnRight) {
			break;
		}
	}
	if (!wallOnLeft && (x <= 0)) { //should this be ==? //also, should this function return false if the lightning is out of bounds?
		wallOnLeft = true;
	}
	if (!wallOnRight && (x + w >= GAME_WIDTH)) {
		wallOnRight = true;
	}

	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		if (CollisionHandler::partiallyCollided(this, HazardManager::getCircleHazard(i))) {
			return false;
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		RectHazard* rh = HazardManager::getRectHazard(i);
		if (rh->getGameID() != this->getGameID()) {
			//TODO: does this care if it's colliding with another version of itself?
			if (CollisionHandler::partiallyCollided(this, rh)) {
				return false;
			}
		}
	}

	return (!(wallOnLeft && wallOnRight) && validLocation());
}

void VerticalLightning::simpleRefreshBolt(int num) {
	double maxVariance = w/4;
	/* lightning bolts are allowed to be in an area that looks like this:
	 * (see HorizontalLightning for a better diagram, then just mentally rotate it)
	 * +-------- /-\ --------+        <- h
	 *         /     \        
	 *      /           \     
	 *   /                 \  
	 * |                     |
	 * |                     |
	 * |                     |
	 *   \                 /
	 *      \           /
	 *         \     /
	 * +-------- \-/ --------+        <- 0
	 * 
	 * ^                     ^
	 * 0                     w
	 *
	 * the region is (from bottom to top) 1/4 triangle, 1/2 rectangle, then 1/4 triangle
	 */

	double deltaY = (bolts[num]->positions[bolts[num]->length*2-1] - bolts[num]->positions[1]) / (bolts[num]->length - 1);
	for (int j = 1; j < bolts[num]->length-1; j++) {
		double xRangeLower = bolts[num]->positions[j*2 - 2] - maxVariance;
		double xRangeUpper = bolts[num]->positions[j*2 - 2] + maxVariance;
		double xMin, xMax;
		if (j < bolts[num]->length/4) { //bottom quarter
			//instead of y=ax+b, use x=(y-b)/a
			xMin = ((deltaY * j) - h/4) / (-.5*h/w);
			xMax = ((deltaY * j) + h/4) / ( .5*h/w);
		} else if (j < bolts[num]->length * 3.0/4.0) { //middle half
			xMin = 0;
			xMax = w;
		} else { //top quarter
			xMin = ((deltaY * j) - 3.0/4.0*h) / ( .5*h/w);
			xMax = ((deltaY * j) - 5.0/4.0*h) / (-.5*h/w);
			//alternatively:
			//xMin = ((deltaY * (j - bolts[num]->length*3.0/4.0)) -   0) / ( .5*h/w);
			//xMax = ((deltaY * (j - bolts[num]->length*3.0/4.0)) - h/2) / (-.5*h/w);
		}
		xRangeLower = (xRangeLower < xMin ? xMin : xRangeLower);
		xRangeUpper = (xRangeUpper > xMax ? xMax : xRangeUpper);
		bolts[num]->positions[j*2+0] = xRangeLower + (xRangeUpper - xRangeLower) * RNG::randFunc2();
	}
}

void VerticalLightning::refreshBolt(int num) {
	RectangularLightning::refreshBolt(num, w, h);
}

void VerticalLightning::draw() const {
	drawBackground();
	drawBolts();
}

void VerticalLightning::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground();
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for VerticalLightning::draw!" << std::endl;
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

void VerticalLightning::poseDraw() const {
	//TODO
}

void VerticalLightning::poseDraw(DrawingLayers layer) const {
	//TODO
}

void VerticalLightning::ghostDraw(float alpha) const {
	//TODO
}

void VerticalLightning::ghostDraw(DrawingLayers layer, float alpha) const {
	//TODO
}

inline void VerticalLightning::drawBackground(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	ColorValueHolder color = getBackgroundColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*background_va, *background_ib, *shader);
}

inline void VerticalLightning::drawBolts(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	if (!currentlyActive) {
		return;
	}

	glLineWidth(2.0f);

	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(1, 1, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	for (int i = 0; i < bolts.size(); i++) {
		//I think the VertexBuffer resizing should happen here, but there would probably be less strain if it happens only when a bullet/tank collides
		//TODO: that ^ should be the preferred way, since only draw() (and initializeGPU()) should do GPU stuff
		/*
		if (bolts[i]->length > bolt_vb_length) {
			local_reinitializeGPU(bolts[i]->length);
		}
		*/
		streamBoltVertices(i); //TODO: fix
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, bolts[i]->length);
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

RectHazard* VerticalLightning::randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv) {
	//minimum/maximum width and height not in argv
	if (WallManager::getNumWalls() == 0) {
		return nullptr; //don't bother trying to see if a vertical lightning could go from edge to edge
	}
	int attempts = 0;
	RectHazard* randomized = nullptr;
	double xpos, ypos, width, height;
	double minHeight = 40, maxHeight = 160;
	do {
		width = RNG::randFunc2() * (24 - 12) + 12;
		//TODO: ability to use an edge
		for (int i = 0; i < WallManager::getNumWalls(); i++) {
			Wall* wa = WallManager::getWall(i);
			xpos = wa->x + RNG::randFunc2() * constrain<double>(wa->w - width, 0, wa->w);
			ypos = wa->y + wa->h;
			int j, wallAttempts = 0;
			do {
				j = RNG::randFunc() * WallManager::getNumWalls();
				wallAttempts++;
			} while ((wallAttempts < 8) && (j == i));
			if (j != i) {
				Wall* otherWall = WallManager::getWall(j);
				height = otherWall->y - ypos;
			} else {
				height = RNG::randFunc2() * (maxHeight - minHeight) + minHeight;
			}
		}
		if ((xpos >= x_start) && (xpos + width <= x_start + area_width) && (ypos >= y_start) && (ypos + height <= y_start + area_height) && (height <= maxHeight) && (height >= minHeight)) {
			RectHazard* testVerticalLightning = new VerticalLightning(xpos, ypos, width, height);
			if (testVerticalLightning->reasonableLocation()) {
				randomized = testVerticalLightning;
				break;
			} else {
				delete testVerticalLightning;
			}
		}
		attempts++;
	} while (attempts < 32);

	return randomized;
}
