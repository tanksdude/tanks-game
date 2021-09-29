#include "horizontallightning.h"
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

VertexArray* HorizontalLightning::background_va;
VertexBuffer* HorizontalLightning::background_vb;
IndexBuffer* HorizontalLightning::background_ib;
bool HorizontalLightning::initialized_GPU = false;

HorizontalLightning::HorizontalLightning(double xpos, double ypos, double width, double height) : RectangularLightning(xpos,ypos,width,height,true) {
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

Circle* HorizontalLightning::getLeftPoint() {
	return new Point(x, y + h/2);
}

Circle* HorizontalLightning::getRightPoint() {
	return new Point(x + w, y + h/2);
}

HorizontalLightning::~HorizontalLightning() {
	//calls ~RectangularLightning(), so this doesn't need to do anything extra
	//clearBolts();

	local_uninitializeGPU(); //I don't know if this is okay, but there isn't an error...
	//uninitializeGPU();
}

bool HorizontalLightning::initializeGPU() {
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
void HorizontalLightning::local_initializeGPU() {
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

void HorizontalLightning::local_reinitializeGPU(int length) { //does not seed the VertexBuffer with values
	delete bolt_va;
	delete bolt_vb;

	float* positions = new float[length*2];
	bolt_vb_length = length;
	
	bolt_vb = VertexBuffer::MakeVertexBuffer(positions, length*2 * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout(2);
	bolt_va = VertexArray::MakeVertexArray(*bolt_vb, layout);

	delete[] positions;
}

bool HorizontalLightning::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;

	initialized_GPU = false;
	return true;
}

void HorizontalLightning::local_uninitializeGPU() {
	delete bolt_va;
	delete bolt_vb;
}

void HorizontalLightning::streamBoltVertices(unsigned int boltNum) const {
	bolt_vb->modifyData(bolts[boltNum]->positions.data(), bolts[boltNum]->length*2 * sizeof(float));
}

RectHazard* HorizontalLightning::factory(int argc, std::string* argv) {
	if (argc >= 4) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double w = std::stod(argv[2]);
		double h = std::stod(argv[3]);
		return new HorizontalLightning(x, y, w, h);
	}
	return new HorizontalLightning(0, 0, 0, 0);
}

void HorizontalLightning::specialEffectCircleCollision(Circle* c) {
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
		DoublePositionHolder intersections = CollisionHandler::circleLineIntersection(c, xpos, ypos, leftPoint->x, leftPoint->y);
		intersectionXL = std::min(intersections.x1, intersections.x2);
		if (c->y < y + h/2) {
			intersectionYL = std::max(intersections.y1, intersections.y2);
		} else {
			intersectionYL = std::min(intersections.y1, intersections.y2);
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
		DoublePositionHolder intersections = CollisionHandler::circleLineIntersection(c, xpos, ypos, rightPoint->x, rightPoint->y);
		intersectionXR = std::max(intersections.x1, intersections.x2);
		if (c->y < y + h/2) {
			intersectionYR = std::max(intersections.y1, intersections.y2);
		} else {
			intersectionYR = std::min(intersections.y1, intersections.y2);
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
	delete leftPoint, rightPoint, circleCenter;

	//TODO: refactor? (it's done, right?)
}

void HorizontalLightning::pushBolt(LightningBolt* l, bool simpleRefresh) {
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

void HorizontalLightning::pushDefaultBolt(int num, bool randomize) {
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

bool HorizontalLightning::validLocation() {
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

bool HorizontalLightning::reasonableLocation() {
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

	return (!(wallOnTop && wallOnBottom) && validLocation());
}

void HorizontalLightning::simpleRefreshBolt(int num) {
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
	for (int j = 1; j < bolts[num]->length-1; j++) {
		//bolts[num]->positions[j*2]   = float(j)/(bolts[num]->length - 1);
		//bolts[num]->positions[j*2+1] = randFunc2();
		double testPoint; //y-position of the new point
		if(j < bolts[num]->length / 4){ //first quarter
			do{
				testPoint = bolts[num]->positions[j*2 - 1] + (randFunc2()*2-1) * maxVariance;
			}while(testPoint <= -2 * (double(j) / bolts[num]->length) + .5 || //"below" the triangle (just in slope-intercept form, nothing special)
			       testPoint >=  2 * (double(j) / bolts[num]->length) + .5);  //"above" the triangle
		}else if(j < bolts[num]->length * 3.0/4.0){ //middle half
			do{
				testPoint = bolts[num]->positions[j*2 - 1] + (randFunc2()*2-1) * maxVariance;
			}while(testPoint >= 1 || testPoint <= 0);
		}else{ //last quarter
			do{
				testPoint = bolts[num]->positions[j*2 - 1] + (randFunc2()*2-1) * maxVariance;
			}while(testPoint <=  2 * (double(j) / bolts[num]->length - 3.0/4.0) + 0 ||
			       testPoint >= -2 * (double(j) / bolts[num]->length - 3.0/4.0) + 1);
		}
		bolts[num]->positions[j*2+1] = testPoint;
	}
	*/

	double deltaX = (bolts[num]->positions[bolts[num]->length*2-2] - bolts[num]->positions[0]) / (bolts[num]->length - 1);
	for (int j = 1; j < bolts[num]->length-1; j++) {
		double yRangeLower = bolts[num]->positions[j*2 - 1] - maxVariance;
		double yRangeUpper = bolts[num]->positions[j*2 - 1] + maxVariance;
		double yMin, yMax;
		if (j < bolts[num]->length/4) { //first quarter
			yMin = -2*h/w * (deltaX * j) + h/2;
			yMax =  2*h/w * (deltaX * j) + h/2;
		} else if (j < bolts[num]->length * 3.0/4.0) { //middle half
			yMin = 0;
			yMax = h;
		} else { //last quarter
			yMin =  2*h/w * (deltaX * (j - bolts[num]->length*3.0/4.0)) + 0;
			yMax = -2*h/w * (deltaX * (j - bolts[num]->length*3.0/4.0)) + h;
		}
		yRangeLower = (yRangeLower < yMin ? yMin : yRangeLower);
		yRangeUpper = (yRangeUpper > yMax ? yMax : yRangeUpper);
		bolts[num]->positions[j*2+1] = yRangeLower + (yRangeUpper - yRangeLower) * RNG::randFunc2();
	}
}

void HorizontalLightning::refreshBolt(int num) {
	RectangularLightning::refreshBolt(num, h, w);
}

void HorizontalLightning::draw() const {
	draw(x, y);
}

void HorizontalLightning::draw(double xpos, double ypos) const {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(w, h, 0, xpos, ypos);
	
	//background:
	//TODO: make drawUnder() a thing
	ColorValueHolder color = getBackgroundColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*background_va, *background_ib, *shader);

	//bolts:
	if (!currentlyActive) {
		return;
	}

	glLineWidth(2.0f);
	color = getBoltColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	MVPM = Renderer::GenerateMatrix(1, 1, 0, xpos, ypos);
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
}

void HorizontalLightning::poseDraw() const {
	//TODO
	return;
}

RectHazard* HorizontalLightning::randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv) {
	//minimum/maximum width and height not in argv
	if (WallManager::getNumWalls() == 0) {
		return nullptr; //don't bother trying to see if a horizontal lightning could go from edge to edge
	}
	int attempts = 0;
	RectHazard* randomized = nullptr;
	double xpos, ypos, width, height;
	double minWidth = 40, maxWidth = 160;
	do {
		height = RNG::randFunc2() * (24 - 12) + 12;
		for (int i = 0; i < WallManager::getNumWalls(); i++) {
			Wall* wa = WallManager::getWall(i);
			xpos = wa->x + wa->w;
			ypos = wa->y + RNG::randFunc2() * constrain<double>(wa->h - height, 0, wa->h);
			int j, wallAttempts = 0;
			do {
				j = RNG::randFunc() * WallManager::getNumWalls();
				wallAttempts++;
			} while ((wallAttempts < 8) && (j == i));
			if (j != i) {
				Wall* otherWall = WallManager::getWall(j);
				width = otherWall->x - xpos;
			} else {
				width = RNG::randFunc2() * (maxWidth - minWidth) + minWidth;
			}
		}
		if ((xpos >= x_start) && (xpos + width <= x_start + area_width) && (ypos >= y_start) && (ypos + height <= y_start + area_height) && (width <= maxWidth) && (width >= minWidth)) {
			RectHazard* testHorizontalLightning = new HorizontalLightning(xpos, ypos, width, height);
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
